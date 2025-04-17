from pathlib import Path



def main(arq_name: Path):
    with open(arq_name, 'r') as file:
        content = file.read()

        # Replace literal '\n' with actual newline characters
        content = content.replace('<br>', '\n').replace("<div>", "").replace("</div>", "")
        
        with open(arq_name.stem + '_clean.txt' , 'w') as file:
            file.write(content)
    
    

if __name__ == '__main__':
    arq_name = Path('input9.txt')
    main(arq_name)