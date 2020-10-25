#! /bin/bash
repo_name="gps-t3"
cd docs
doxygen Doxyfile
cd latex
make
cd ../..
echo "# Directory Structure" > directorySctructure.md
echo "\`\`\`" >> directoryStructure.md
tree ../${repo_name}/src >> directorySctructure.md
tree ../${repo_name}/headers >> directorySctructure.md
directorySctructure.md
echo "\`\`\`" >> directorySctructure.md