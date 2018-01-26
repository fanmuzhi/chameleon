branch=develop/qbo

# update cmlFpSensorEngine
cd ./cmlFpSensorEngine
git pull $branch
cd ..

# update cmlFpTestEngine 
cd ./cmlFpTestEngine
git pull $branch
cd ..

# update cmlFpApp
cd ./cmlFpApp
git pull $branch
cd ..

# show status
git status
