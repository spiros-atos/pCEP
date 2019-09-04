#!/bin/bash
pm2 ls
/usr/local/lib/node_modules/pm2-gui/pm2-gui start
pm2 start -f -n my_bcep ./bcep -- "$@"
pm2 logs my_bcep
