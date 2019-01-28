while true;do
    make 
    sleep 1s
    valgrind ./lab5 -u 3
    valgrind ./lab5 -u 5
    valgrind ./lab5 -u 9
    sleep 1s
    clear
    echo "next try"
    echo "==================="
    echo "| System restart  |"
    echo "==================="
    sleep 1s
        
done
