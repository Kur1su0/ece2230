while true;do
    valgrind ./lab2 < mytestinput.txt #| tee mylog.log
    sleep 7s
    clear
    echo "next try"
    echo "==================="
    echo "| System restart  |"
    echo "==================="
    sleep 1s
done
