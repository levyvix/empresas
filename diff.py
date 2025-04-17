import difflib

def read_file(file_path):
    with open(file_path, 'r') as file:
        # remove last \n
        return file.readlines()[:-1]

def compare_files(file1, file2):
    file1_lines = read_file(file1)
    file2_lines = read_file(file2)

    diff = difflib.unified_diff(
        file1_lines, file2_lines,
        fromfile=file1,
        tofile=file2,
        lineterm=''
    )

    for line in diff:
        print(line)

if __name__ == "__main__":
    file1 = 'output.txt'
    file2 = 'compare.txt'
    compare_files(file1, file2)