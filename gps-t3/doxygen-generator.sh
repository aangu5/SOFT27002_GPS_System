#! /bin/bash
repo_name="gps-t3"
cd docs
doxygen Doxyfile
cd latex
make
cd ../..
echo "# Directory Structure" > directoryStructure.md
echo "\`\`\`" >> directoryStructure.md
tree ../${repo_name}/src >> directoryStructure.md
tree ../${repo_name}/headers >> directoryStructure.md
directoryStructure.md
echo "\`\`\`" >> directoryStructure.md