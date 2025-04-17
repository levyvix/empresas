import os
import zipfile
import sys


if len(sys.argv) > 1:
    num = int(sys.argv[1])
    if num == 1:
        print("PROGRAMA: EMPRESAS")
    elif num == 2:
        print("PROGRAMA: JOGADORES")
else:
    print("usage: python zip_files.py <int>")
    

# Define the name of the output zip file
zip_filename = f'source_files_{num}.zip'

# Create a new zip file
if os.path.exists(zip_filename):
    print("o arquivo ja existe, apagando...")
    os.remove(zip_filename)

with zipfile.ZipFile(zip_filename, 'w') as zipf:
    # Iterate over all files in the current directory
    for file in os.listdir('.'):
        # Check if the file ends with .c or .h
        if file.endswith('.c') or file.endswith('.h'):
            if num == 1 and file == 'main2.c':
                continue
            elif num ==2 and file == 'main.c':
                continue
            # Add the file to the zip archive
            zipf.write(file)
            print(f'Added {file} to {zip_filename}')

print(f'All .c and .h files have been zipped into {zip_filename}')