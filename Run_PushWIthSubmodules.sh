#git push --recurse-submodules=on-demand origin main:main
#git config credential.helper store
git submodule foreach "git push origin main:main"
git push origin main:main
