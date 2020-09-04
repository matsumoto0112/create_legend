import glob
import os
import shutil
import subprocess


def find_files(ext):
    files = glob.glob('*/**/*{}'.format(ext))
    return files


def create_file_pairs(ext):
    res = []
    files = find_files(ext)
    for file in files:
        file_basename = os.path.basename(file)
        filename_rm_ext = os.path.splitext(file_basename)[0]
        filename_rm_ext_upper = filename_rm_ext.upper()
        res.append((filename_rm_ext_upper, file_basename))
    return res


def create_file_names(ext):
    pairs = create_file_pairs(ext)
    res = []
    for pair in pairs:
        s = 'constexpr name {} = L"{}";'.format(pair[0], pair[1])
        res.append(s)
    return res


def write_resource_names_to_temp_txt():
    path = 'resource_names.txt'
    with open(path, mode='w') as f:
        for t in create_file_names('.png'):
            f.write(t + '\n')

        f.write('\n')

        for t in create_file_names('.wav'):
            f.write(t + '\n')

        f.write('\n')

        for t in create_file_names('.glb'):
            f.write(t + '\n')


def main():
    if os.path.exists('./archive.bin'):
        os.remove('archive.bin')

    if os.path.exists('./assets'):
        shutil.rmtree('./assets')

    asset_path = os.path.join('..', 'legend', 'assets')
    print(asset_path)
    shutil.copytree(asset_path, './assets')

    subprocess.call('Archive.exe')
    shutil.copyfile('archive.bin', asset_path + '\\archive.bin')
    write_resource_names_to_temp_txt()

    os.remove('archive.bin')
    shutil.rmtree('assets')


if __name__ == "__main__":
    main()
