
    // // Add all points to some clusters
    // int num_clusters = 0;
    // int was_there = 0;
    // init_cluster(clusters, 0);
    // append_cluster(&clusters[0], parr[0]);
    // num_clusters++;
    // float minimum, temp;
    // // for (int i = 0; i < num_of_points; i++){
    // //     printf("I am i: %d", i);
    // //     minimum = INFINITY;
    // //     for (int j = 0; j < num_of_points; j++){
    // //         if (i != j){
    // //             temp = obj_distance(&parr[i], &parr[j]);
    // //             if (temp < minimum){
    // //                 minimum = temp;
    // //                 temp_obj = parr[j];
    // //             }
    // //         }
    // //     }
    // //     for (int j = 0; j < num_clusters && !was_there; j++){
    // //         for (int k = 0; k < clusters[j].size; k++){
    // //             if (clusters[j].obj[k].id == parr[i].id){
    // //                 append_cluster(&clusters[j], temp_obj);
    // //                 was_there = 1;
    // //                 break;
    // //             }
    // //         }
    // //     }
    // //     if (!was_there){
    // //         init_cluster(clusters, 0);
    // //         num_clusters++;
    // //         append_cluster(&clusters[num_clusters], temp_obj);
    // //     }
    // // }