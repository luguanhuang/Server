#!/user/bin/pyton
# -*- coding: UTF-8 -*-

import random

count = 20000
span = "\t"
professions = ["竞技场战士", "竞技场弓箭手", "竞技场法师", "竞技场牧师"]

file = open("ArenaNpc.txt", "w+")
id = 1
while(id <= count):
    temp = random.randint(0, len(professions) - 1);
    line = str(id) + span + professions[temp]+str(id) + span + str(temp+1) + "\n"
    print line
    file.write(line)
    id = id + 1;

file.close()

