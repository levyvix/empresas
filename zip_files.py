import os
import zipfile

# Define the name of the output zip file
zip_filename = 'source_files.zip'

# Create a new zip file
if os.path.exists(zip_filename):
    print("o arquivo ja existe, apagando...")
    os.remove(zip_filename)

with zipfile.ZipFile(zip_filename, 'w') as zipf:
    # Iterate over all files in the current directory
    for file in os.listdir('.'):
        # Check if the file ends with .c or .h
        if file.endswith('.c') or file.endswith('.h'):
            # Add the file to the zip archive
            zipf.write(file)
            print(f'Added {file} to {zip_filename}')

print(f'All .c and .h files have been zipped into {zip_filename}')