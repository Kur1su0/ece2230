sh e.sh |tee log/e.log
sh e_500.sh |tee log/e_500.log
sh eq.sh |tee log/eq.log
./lab4 -e -d |tee log/d.log
./lab4 -e -d -r 0 |tee log/eq_d.log
