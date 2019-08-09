#!/bin/bash -e

# Settings
REPO_PATH="https://github.com/rayslava/tgqif.git"
HTML_PATH=doc/html
CHANGESET=$(git rev-parse --verify HEAD)

# Get a clean version of the HTML documentation repo.
rm -rf ${HTML_PATH}
mkdir -p ${HTML_PATH}
git clone -b gh-pages "${REPO_PATH}" --single-branch ${HTML_PATH}
set -x

# Prepare configuration and README file
sed -ie "/PROJECT_NUMBER/s/=.*$/= ${CHANGESET}/" Doxyfile

# rm all the files through git to prevent stale files.
cd ${HTML_PATH}
git rm -rf .
cd -

# Generate the HTML documentation.
make doxygen JOBS=3

# Create and commit the documentation repo.
cd ${HTML_PATH}
git add .
git commit -m "Automated documentation build for changeset ${CHANGESET}."
git push "${REPO_PATH}" gh-pages
cd -
