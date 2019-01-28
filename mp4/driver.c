 else if (dprms.UnitDriver == 5)
    {
        printf("\n----- Begin unit driver 5 -----\n");
        /* .
        Test Driver 5 create the ascendind size of free blocks ,realloc 
        the ascending size of blocks, difference search strategies show 
        the big difference in result after alloc stage
         */

        int unit_size = sizeof(chunk_t);
        int units_in_first_page = PAGESIZE/unit_size;
        assert(units_in_first_page * unit_size == PAGESIZE);
        printf("There are %d units per page, and the size of chunk_t is %d bytes\n", 
                units_in_first_page, unit_size); 

        int *p1, *p2, *p3,*p4,*p5,*p6;
        int num_bytes_1, num_bytes_2, num_bytes_3,num_bytes_4,num_bytes_5,num_bytes_6;

        // allocate three quarters of one page
        num_bytes_1 = (units_in_first_page/4*3-1)*unit_size;
        p1 = (int *) Mem_alloc(num_bytes_1);
        printf("first: %d bytes (%d units) p=%p \n", 
                num_bytes_1, num_bytes_1/unit_size, p1);
        Mem_print();

        // allocate three quarter of one page plus more unit size 
        num_bytes_2 = (units_in_first_page/4*3-5)*unit_size;
        p2 = (int *) Mem_alloc(num_bytes_2);
        printf("second: %d bytes (%d units) p=%p \n", 
                num_bytes_2, num_bytes_2/unit_size, p2);
        Mem_print();

        // allocate three quarter of one page plus more unit siz
        num_bytes_3 = (units_in_first_page/4*3-8)*unit_size;
        p3 = (int *) Mem_alloc(num_bytes_3);
        printf("third: %d bytes (%d units) p=%p \n", 
                num_bytes_3, num_bytes_3/unit_size, p3);
        Mem_print();
        
        // allocate one quarter of one page
        num_bytes_4 = (units_in_first_page/4-1)*unit_size;
        p4 = (int *) Mem_alloc(num_bytes_4);
        printf("forth: %d bytes (%d units) p=%p \n", 
                num_bytes_4, num_bytes_4/unit_size, p4);
        Mem_print();
        
        // allocate the size equal to the rest of the p2
        num_bytes_5 = (units_in_first_page/4+3)*unit_size;
        p5 = (int *) Mem_alloc(num_bytes_5);
        printf("fifth: %d bytes (%d units) p=%p \n", 
                num_bytes_5, num_bytes_5/unit_size, p5);
        Mem_print();
       
        // allocate the size equal to the rest of the p2
        num_bytes_6 = (units_in_first_page/4+6)*unit_size;
        p6 = (int *) Mem_alloc(num_bytes_6);
        printf("sixth: %d bytes (%d units) p=%p \n", 
                num_bytes_6, num_bytes_6/unit_size, p6);
        Mem_print();
        printf("unit driver 5: above Mem_print shows free list\n");

        

        // next put the memory back into the free list:

        printf("first free of first one whole page p=%p \n", p3);
        Mem_free(p3);
        Mem_print();

        printf("second free of third one whole page p=%p \n", p2);
        Mem_free(p2);
        Mem_print();

        printf("third free of second one whole page p=%p \n", p4);
        Mem_free(p4);
        Mem_print();

        printf("forth free of second one whole page p=%p \n", p5);
        Mem_free(p5);
        Mem_print();
        
        printf("fifth free of second one whole page p=%p \n", p6);
        Mem_free(p6);
        Mem_print();
        
        printf("sixth free of second one whole page p=%p \n", p1);
        Mem_free(p1);
        Mem_print();
        
        printf("unit driver 5 has returned all memory to free list\n");
        Mem_print();
        Mem_stats();
        printf("\n----- End unit test driver 5 -----\n");
    }
