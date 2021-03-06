#include "src/search/search_manager.h"

#include <algorithm>
#include <iostream>

#include "src/enemy/boss.h"
#include "src/enemy/enemy.h"
#include "src/enemy/enemy_type.h"
#include "src/game/game_device.h"
#include "src/object/fragment.h"
#include "src/skill/skill_item_box.h"

namespace legend {
namespace search {

// コンストラクタ
SearchManager::SearchManager() {}

// デスクトラクタ
SearchManager::~SearchManager() {
  auto searchCount = search_list_.size();
  for (i32 i = 0; i < searchCount; i++) {
    search_list_.erase(search_list_.begin());
  }
}

// 初期設定
void SearchManager::Initialize(actor::IActorMediator* mediator) {
  mediator_ = mediator;
}

// 生成
void SearchManager::Make(std::filesystem::path filepath) {
  search_list_.clear();

  // 探索箇所テキスト取得
  auto indexs_ = LoadStringStageData(filepath);
  if (indexs_.empty() || indexs_[0] == "error") {
    MY_LOG(L"データが読み込まれていないか、読み込みに失敗しています。");
    return;
  }

  // 探索箇所パラメータ
  struct paramater {
    //! 番号
    i32 index;
    //! 座標
    math::Vector3 pos;
    //! 分岐先番号
    std::vector<i32> indexs;
  };

  std::vector<paramater> searchs;
  for (auto&& index : indexs_) {
    //文字列を分割
    std::vector<std::string> infomation = StringSplit(index, ',');

    if (infomation[0] == "search") {
      paramater search;
      auto x = std::stof(infomation[1].c_str());
      auto y = std::stof(infomation[2].c_str());
      auto z = std::stof(infomation[3].c_str());

      search.index = static_cast<i32>(searchs.size());
      search.pos = math::Vector3(x, y, z);

      for (i32 i = 4; i < infomation.size(); i++) {
        search.indexs.emplace_back(std::stoi(infomation[i].c_str()));
      }
      Add(search.pos);
      searchs.emplace_back(search);
    }
  }

  // 分岐先設定
  for (auto search : searchs) {
    SetBranch(search.index, search.indexs);
  }
}

// テキストの読み込み処理
std::vector<std::string> SearchManager::LoadStringStageData(
    std::filesystem::path filepath) {
  //ファイルパスからテキストデータを読み込み
  std::ifstream ifstream(filepath);
  std::vector<std::string> indexs;
  if (ifstream.fail()) {
    MY_LOG(L"ステージデータを開けませんでした。");
    indexs.push_back("error");
    return indexs;
  }
  std::string index;
  while (std::getline(ifstream, index)) {
    indexs.push_back(index);
  }

  return indexs;
}

// StringをStringリストへ変換
std::vector<std::string> SearchManager::StringSplit(const std::string& string,
                                                    char border) {
  std::vector<std::string> elements;
  std::stringstream ss(string);
  std::string item;

  while (std::getline(ss, item, border)) {
    if (item.empty()) continue;

    elements.push_back(item);
  }

  return elements;
}

// 座標から分岐点追加
void SearchManager::Add(math::Vector3 _position) {
  auto search = std::make_unique<SearchAI>(_position);
  search_list_.emplace_back(std::move(search));
}

// 座標リストから分岐点追加
void SearchManager::Add(std::vector<math::Vector3> _positions) {
  for (auto p : _positions) {
    Add(p);
  }
}

// 座標から分岐点追加
void SearchManager::SetBranch(i32 index, std::vector<i32> branch) {
  if (index < 0 || search_list_.size() <= index) {
    return;
  }
  for (i32 i = 0; i < branch.size(); i++) {
    if (branch[i] < 0 || search_list_.size() <= branch[i]) {
      return;
    }
    auto add = search_list_[branch[i]].get();
    search_list_[index].get()->AddBranch(add);
  }
}

// 次の経路座標
math::Vector3 SearchManager::NextSearch(
    bullet::Collider* _enemy, std::vector<bullet::Collider*> _enemys) {
  if (mediator_->GetPlayer() == nullptr) {
    return math::Vector3::kZeroVector;
  }

  ignore_enemy_ = _enemy;
  enemys_ = _enemys;
  auto start = _enemy->GetOwner()->GetTransform().GetPosition();
  auto end = mediator_->GetPlayer()->GetPosition();
  auto point = start + (end - start) / 2.0f;
  course_list_.clear();

  // 衝突したら探索経路
  if (OnCollision(start, end) ||
      !OnCollision(point, point + math::Vector3::kDownVector * 15.0f)) {
    SetCourse(NearSearch(start), NearSearch(end));
    ChaseCourse();
    end = (course_list_.size() <= 0 ? end : course_list_[0]->GetPosition());
  }

  return end;
}

// 経路探索
void SearchManager::SetCourse(SearchAI* sStart, SearchAI* sEnd) {
  // 経路初期化
  course_list_.clear();
  if ((sStart == nullptr) || (sEnd == nullptr)) return;

  //! 始点
  auto start = SearchCourse(sStart);
  //! 経路リスト
  auto searched = std::vector<SearchCourse>{start};
  //! 経路番号リスト
  auto root = std::vector<i32>{};

  // 探索処理
  for (i32 i = 0; i < searched.size(); i++) {
    // 探索先を取得
    std::vector<SearchCourse> children = {};
    auto branch = GetBaseSearch(searched[i].GetBaseSearch())->GetBranch();
    for (auto b : branch) {
      children.emplace_back(SearchCourse(GetBaseSearch(b), i));
    }

    // 削除対象に被る場合、探索先から削除
    for (i32 s = 0; s < searched.size(); s++) {
      for (i32 j = 0; j < children.size(); j++) {
        auto search = GetBaseSearch(searched[s].GetBaseSearch());
        auto child = GetBaseSearch(children[j].GetBaseSearch());
        if (search == child) {
          children.erase(children.begin() + j);
          j--;
        }
      }
    }

    // 長さからソート
    auto indexs = std::vector<i32>{};
    for (i32 c_index = 0; c_index < children.size(); c_index++) {
      bool is_size = (indexs.size() <= 0);
      if (indexs.size() <= 0) {
        indexs.emplace_back(c_index);
      } else {
        for (i32 i_index = 0; i_index < indexs.size(); i_index++) {
          float i_length = (children[indexs[i_index]].Length(searched));
          float c_length = children[c_index].Length(searched);
          if (c_length < i_length) {
            indexs.insert(indexs.begin() + i_index, c_index);
            break;
          } else if (i_index + 1 <= indexs.size()) {
            indexs.emplace_back(c_index);
            break;
          }
        }
      }
    }

    // 衝突判定
    for (i32 j = 0; j < children.size(); j++) {
      auto c_index = indexs[j];
      auto sPos = searched[i].GetBaseSearch()->GetPosition();
      auto ePos = children[c_index].GetBaseSearch()->GetPosition();
      auto length = children[c_index].Length(searched);
      // 衝突しなかったら探索箇所に追加
      if (!OnCollision(sPos, ePos)) {
        searched.emplace_back(
            SearchCourse(GetBaseSearch(children[j].GetBaseSearch()), i));
      }
    }

    // 最終地点と被ったらルートに追加
    if (GetBaseSearch(searched[i].GetBaseSearch()) == GetBaseSearch(sEnd)) {
      root = searched[i].GetParents(searched);
      break;
    }
  }

  // ルートに追加されている分岐点の座標をルートに追加
  for (auto r : root) {
    course_list_.emplace_back(GetBaseSearch(searched[r].GetBaseSearch()));
  }
}

// 追跡経路
void SearchManager::ChaseCourse() {
  if (course_list_.size() <= 0) return;

  // 一つ飛ばしに衝突判定を行い、衝突しなければ間の座標を省く
  auto start = ignore_enemy_->GetOwner()->GetTransform().GetPosition();
  for (i32 i = 1; i < course_list_.size(); i++) {
    auto end = course_list_[i]->GetPosition();
    auto point = start + (end - start) / 2.0f;
    // 衝突判定
    if (!OnCollision(start, end) &&
        OnCollision(point, point + math::Vector3::kDownVector * 15.0f)) {
      course_list_.erase(course_list_.begin() + i - 1);
      i--;
    }
  }
}

// 座標から近場の探索箇所を取得
SearchAI* SearchManager::NearSearch(math::Vector3 _position) {
  if (search_list_.size() <= 0) {
    return nullptr;
  }

  // 自身の座標に近く、間に衝突店がないものを取得
  SearchAI* result = nullptr;
  for (i32 i = 0; i < search_list_.size(); i++) {
    if ((result == nullptr) ||
        ((search_list_[i]->GetPosition() - _position).Magnitude() <
         (result->GetPosition() - _position).Magnitude())) {
      // 衝突判定
      if (!OnCollision(_position, search_list_[i]->GetPosition())) {
        result = search_list_[i].get();
      }
    }
  }

  return result;
}

// 原点のポインタを取得
SearchAI* SearchManager::GetBaseSearch(SearchAI* _search) {
  // ポインタを取得
  for (auto& s : search_list_) {
    if (s.get() == _search) return s.get();
  }
  return nullptr;
}

// 衝突情報を取得
bool SearchManager::OnCollision(math::Vector3 start, math::Vector3 end) {
  // 衝突判定
  const auto raycast = mediator_->RayCast(start, end);
  auto objs = raycast.m_collisionObjects;

  // 移動判定
  enemy::enemy_type::MoveType move_type;
  auto ea = dynamic_cast<enemy::EnemyActor*>(ignore_enemy_->GetOwner());
  if (ea != nullptr) {
    move_type = ea->GetMoveType();
  }

  for (i32 index = 0; index < objs.size(); index++) {
    bullet::Collider* act =
        static_cast<bullet::Collider*>(objs[index]->getUserPointer());
    // 稼働中の敵との衝突を無視
    if (act->GetOwner() == ignore_enemy_->GetOwner()) continue;
    // プレイヤーとの衝突を無視
    if (act->GetOwner() == mediator_->GetPlayer()->GetCollider()->GetOwner())
      continue;
    // 敵が直進していく場合、ほかの敵との衝突を無視
    if (move_type == enemy::enemy_type::Move_Straight) {
      if (dynamic_cast<enemy::EnemyActor*>(act->GetOwner()) != nullptr) {
        continue;
      }
    }
    // スキルとの衝突を無視
    if (dynamic_cast<skill::Skill*>(act->GetOwner()) != nullptr) {
      continue;
    }
    // スキルボックスとの衝突を無視
    if (dynamic_cast<skill::SkillItemBox*>(act->GetOwner()) != nullptr) {
      continue;
    }

	// 消しカスとの衝突を無視
    if (dynamic_cast<object::Fragment*>(act->GetOwner()) != nullptr) {
      continue;
    }
    return true;
  }
  return false;
}

// デバッグ描画
void SearchManager::DebugDraw(bullet::PhysicsField* physics_field_) {
  for (i32 i = 0; i < search_list_.size(); i++) {
    search_list_[i]->DebugDraw(physics_field_);
  }
}

}  // namespace search
}  // namespace legend