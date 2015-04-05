#!/bin/bash
for i in `seq 1 10`;
do
   ./tp exp3.in salida.txt 3
done 

for j in `seq 1 10`;
do
   ./tp exp3.in salida.txt 2
done 
