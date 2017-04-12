branch=master

# checkout to branch
git checkout $branch

# make sure the main git is in latest status
git pull

# update the submodule based on the main git in server
git submodule update --init --recursive
