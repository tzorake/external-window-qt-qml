import re
import os
import fnmatch
import subprocess

def find_files_with_extensions(directory, extensions, output_file):
    with open(output_file, 'w') as f:
        for root, _, files in os.walk(directory):
            for extension in extensions:
                for filename in fnmatch.filter(files, f'*{extension}'):
                    f.write(os.path.join(root, filename) + '\n')

def apply_regexes(content, regex_pairs):
    """Apply multiple regex search-replace pairs to the given content."""
    for search_regex, replace_regex in regex_pairs:
        content = re.sub(search_regex, replace_regex, content, flags=re.MULTILINE)
    return content

def replace_in_file(file_path, regex_pairs):
    """Replace occurrences using multiple regex pairs in a file."""
    with open(file_path, 'r', encoding='latin-1') as file:
        content = file.read()

    new_content = apply_regexes(content, regex_pairs)

    if new_content != content:
        with open(file_path, 'w', encoding='latin-1') as file:
            file.write(new_content)
        print(f"Updated: {file_path}")
    else:
        print(f"No changes: {file_path}")

if __name__ == "__main__":
    directory_path = '.\\project'
    extensions = ['.js', '.html', '.css', ".ttf", ".png", ".jpg", ".svg"]
    output_file = 'list.txt'
    find_files_with_extensions(directory_path, extensions, output_file)
    print(f"File paths written to {output_file}")

    regex_pairs = [
        (r"^(.+)$", r'"\1",'),
        (r"\\", r"/"),
    ]

    dirpath = "."

    file_path = os.path.join(dirpath, "list.txt")
    replace_in_file(file_path, regex_pairs)

    with open(file_path, "r") as file:
        replacement = file.read()
        replacement = replacement.rstrip(",\n")
        lines = replacement.splitlines()
        replacement = "\n".join(f"\t\t\t{line}" for line in lines)

    file_path = os.path.join(dirpath, "package.json.template")

    with open(file_path, "r") as file:
        contents = file.read()

    updated_contents = contents.replace("<? FILES ?>", replacement)

    file_path = os.path.join(dirpath, "package.json")

    with open(file_path, "w") as file:
        file.write(updated_contents)
    
    command = 'start cmd /c "npm run build"'
    subprocess.Popen(command, shell=True)
