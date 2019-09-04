#!/bin/bash
# -*- ENCODING: UTF-8 -*-
echo Starting PM2-GUI WEB...
pm2 ls
pm2-gui start
echo Starting CEP with PM2
pm2 start -f -n my_bcep ./bcep -- "$@"
pm2 logs my_bcep
