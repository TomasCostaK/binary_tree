//
// Tomás Oliveira e Silva, AED,  November 2018
//
// 86182 - Rui Coelho
// 89016 - Tomás Costa
// 89059 - João Carvalho
//
//empirical study of random ordered binary trees
//

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "elapsed_time.h"


//
// each node of our ordered binary tree will store a long integer
//
// the root of the tree should be declared as follows (set initially to an empty tree):
//
//   tree_node *root = NULL;
//

typedef struct tree_node
{
  struct tree_node *left;   // pointer to the left branch (a sub-tree)
  struct tree_node *right;  // pointer to the right branch (a sub-tree)
  struct tree_node *parent; // pointer to the parent node (NULL for the root of the tree)
  long data;                // the data item (we use a long here)
}
tree_node;


//
//
//
//

//
// insert a node in the tree (it is assumed that the tree does not store repeated data)
//
// use it as follows (example):
//
//   insert_node(&root,&new_node);
//

static void insert_node(tree_node **link,tree_node *n)
{
  tree_node *parent;

  parent = NULL;
  while(*link != NULL)
  {
    if(n->data == (*link)->data)
    {
      fprintf(stderr,"insert_node: %ld is already in the tree\n",n->data);
      exit(1);
    }
    parent = *link;
    link = (n->data < (*link)->data) ? &((*link)->left) : &((*link)->right); // select branch
  }
  *link = n;
  n->parent = parent;
  n->left = n->right = NULL;
}


//
// count the number of leaves of the tree
//
// use if as follows (example):
//
//   int n_leaves = count_leaves(root);
//


static int count_leaves(tree_node *link)
{
  int n_leaves = 0;

  //caso node dado seja nulo
  if(link == NULL)
    return 0;
  //se nao tiver ponteiro para a esquerda nem direita e pq e uma folha
  if (link->left == NULL && link->right == NULL)
    n_leaves++;
  //caso nao seja folha, incrementamos o n leaves primeiro a esquerda e dps a direita
  else {
    //if existe link->left return leaves += count_leaves, senao 0
    n_leaves += (link->left)? count_leaves(link->left):0;
    //if existe link->right return leaves += count_leaves, senao 0
    n_leaves += (link->right)? count_leaves(link->right):0;
  }

  return n_leaves;
  //return 2;
}


//
// compute the height of the tree
//
// use if as follows (example):
//
//   int height = tree_height(root);
//

static int tree_height(tree_node *link)
{
  int height = 1;
  
  //caso link seja null
  if(link == NULL)
    return 0;

  // //caso nao haja ponteiro para esquerda nem direita chegamos a uma folha, entao damos return a 1, que e esse nivel
  // if (link->left == NULL && link->right == NULL)
  //   return height;

  // //caso so haja node para a direita
  // else if(link->left == NULL)
  //   return height += tree_height(link->right);
  
  // //caso so haja node para a esquerda
  // else if(link->right == NULL)
  //   return height += tree_height(link->left);

  //caso hajam 2 nodes, fazemos o caminho para os dois e retornamos o que tem mais niveis
  //else
  return height += (tree_height(link->left) >= tree_height(link->right)) ? tree_height(link->left) : tree_height(link->right);

}
//
// recursive function used to search for the location of a data item
//
// use if as follows (example):
//
//   tree_node *node = search_tree(root,data);
//

static int search_counter;

tree_node *search_tree(tree_node *link,long data)
{
  search_counter++;
  if(link == NULL)
    return NULL;
  if(link->data == data)
    return link;
  //Se for mais pequeno que o data do node atual ir par a esquerda, senao vamos para a direita
  return search_tree((data < link->data) ? link->left : link->right,data);
}


//
// assuming that each data item is searched for with equal probability, compute the average number
// of recursive function calls to the search_tree() function when
// 1) the search is successful (a hit)
// 2) the search is not successful (a miss)
//
// use them as follows (example):
//
//   double average_calls_on_hit = (double)count_function_calls_on_hit(root,0) / (double)number_of_nodes;
//   double average_calls_on_miss = (double)count_function_calls_on_miss(root,0) / (double)number_of_nodes;
//

static int count_function_calls_on_hit(tree_node *link,int level)
{
  int count;
  // TO DO: delete the next line of code and place your code here
  // if link != null, existe node, o count incrementa o level + 1 = height
  //search_counter tem numero de vezes que func e chamada, root e nivel 0
  if (link == NULL)
    return 0;
  count=level+1;
  return (count += (count_function_calls_on_hit(link->left,level+1) + count_function_calls_on_hit(link->right,level+1)));
}

static int count_function_calls_on_miss(tree_node *link,int level)
{
  // TO DO: delete the next line of code and place your code here
  int count=0;
  // TO DO: delete the next line of code and place your code here
  // if link != null, existe node, o count incrementa o level + 1 = height
  //search_counter tem numero de vezes que func e chamada, root e nivel 0
  if (link == NULL)
    return level+1; 
  else
    return (count += (count_function_calls_on_miss(link->left,level+1) + count_function_calls_on_miss(link->right,level+1)));
}


//
// random permutation of the n numbers 1, 3, 5, ..., 2*n-1
//
// use if as follows (example):
//
//   int n = 100;
//   int a[n];
//   rand_perm(n,&a[0]);
//

static void rand_perm(int n,int *a)
{
  int i,j,k;

  for(i = 0;i < n;i++)
    a[i] = 2 * i + 1;
  for(i = n - 1;i > 0;i--)
  {
    j = (int)floor((double)(i + 1) * (double)rand() / (1.0 + (double)RAND_MAX)); // range 0..i
    k = a[i];
    a[i] = a[j];
    a[j] = k;
  }
}
//  OUTPUT PARA 100000 nodes
//                                   data for 1000 random trees
//           maximum tree height           number of leaves          calls on hit    calls on miss
//         ----------------------  -------------------------------  --------------  --------------
//      n  min max    mean    std    min   max       mean      std     mean    std     mean    std
// ------  --- --- ------- ------  ----- ----- ---------- --------  ------- ------  ------- ------
//     10    4   9  5.6660 0.9276      2     5     3.6550   0.6663   3.4496 0.3929   5.0451 0.3572
//     13    4  11  6.4760 1.0166      2     7     4.6090   0.7925   3.8766 0.4382   5.5283 0.4069
//     16    5  11  6.9840 1.0815      3     8     5.6690   0.8351   4.1732 0.4582   5.8689 0.4313
//     20    5  12  7.7730 1.2106      4    10     7.0200   0.9755   4.5858 0.5130   6.3198 0.4886
//     25    6  14  8.4380 1.2273      6    12     8.6610   1.0909   4.9137 0.4921   6.6862 0.4731
//     32    6  18  9.3130 1.3293      7    15    10.9980   1.2075   5.3568 0.5149   7.1642 0.4993
//     40    7  15 10.0490 1.3866     10    18    13.7370   1.3667   5.7827 0.5531   7.6172 0.5396
//     50    8  17 10.8290 1.4669     12    22    16.9810   1.5280   6.1656 0.5472   8.0251 0.5365
//     63    8  17 11.6550 1.4415     16    26    21.2870   1.7374   6.6122 0.5616   8.4932 0.5528
//     79    9  19 12.4280 1.5234     20    32    26.6840   1.9251   7.0242 0.5741   8.9239 0.5669
//    100   10  19 13.2690 1.5462     26    40    33.6090   2.0890   7.4765 0.5943   9.3926 0.5884
//    126   10  21 14.0990 1.6520     34    50    42.4260   2.4001   7.9116 0.6199   9.8414 0.6150
//    158   11  24 14.8660 1.6613     45    61    52.8920   2.6496   8.3390 0.6266  10.2803 0.6227
//    200   12  22 15.9220 1.7105     56    75    66.9240   3.0173   8.8078 0.6067  10.7590 0.6037
//    251   13  26 16.7430 1.7723     73    94    83.8800   3.4845   9.2634 0.6333  11.2226 0.6308
//    316   14  26 17.4770 1.8252     94   118   105.4940   3.8108   9.7014 0.6461  11.6677 0.6441
//    398   14  26 18.3540 1.8299    119   147   133.0570   4.2128  10.1573 0.6345  12.1293 0.6329
//    501   15  27 19.4450 1.9050    151   184   167.4320   4.6257  10.6527 0.6454  12.6295 0.6441
//    631   16  28 20.2120 1.8818    195   227   210.6580   5.2072  11.0874 0.6595  13.0683 0.6584
//    794   17  28 21.1270 1.8727    245   283   265.0850   5.7329  11.5475 0.6390  13.5317 0.6382
//   1000   18  28 22.0320 1.9321    314   352   333.4960   6.7206  11.9567 0.6155  13.9437 0.6149
//   1259   18  30 22.9100 1.9854    390   443   420.3390   7.2932  12.4196 0.6390  14.4090 0.6385
//   1585   19  32 23.9210 1.9034    504   556   529.2560   8.6191  12.9113 0.6535  14.9025 0.6531
//   1995   20  34 24.7630 1.9972    635   702   665.1490   9.3587  13.3573 0.6293  15.3501 0.6290
//   2512   21  35 25.6390 2.0007    801   870   837.5910  10.2995  13.8255 0.6370  15.8196 0.6368
//   3162   22  34 26.6500 1.9818   1012  1093  1054.6180  11.2465  14.2711 0.6357  16.2663 0.6355
//   3981   23  36 27.4670 2.0719   1275  1364  1327.6370  13.0179  14.7190 0.6509  16.7151 0.6507
//   5012   23  36 28.4960 2.1133   1622  1716  1671.1720  14.4025  15.2136 0.6837  17.2104 0.6836
//   6310   25  37 29.4380 2.0504   2049  2156  2103.9750  16.5256  15.6602 0.6573  17.6576 0.6572
//   7943   25  40 30.3810 2.1568   2580  2708  2647.0270  19.0916  16.1093 0.6480  18.1071 0.6479
//  10000   26  41 31.2600 2.0848   3264  3395  3333.4050  21.0672  16.5935 0.6298  18.5917 0.6298
//  12589   28  41 32.1130 2.0981   4123  4293  4197.9490  23.3098  17.0225 0.6441  19.0211 0.6441
//  15849   28  42 33.1390 2.0952   5201  5361  5283.8170  26.1192  17.5018 0.6746  19.5007 0.6746
//  19953   29  42 33.9520 2.0109   6552  6766  6650.1380  29.7318  17.9617 0.6345  19.9608 0.6344
//  25119   30  47 34.9060 2.1178   8264  8478  8373.7560  34.6661  18.4154 0.6708  20.4146 0.6708
//  31623   31  44 35.8550 2.1932  10427 10652 10541.4810  36.8547  18.8874 0.6488  20.8868 0.6488
//  39811   32  49 36.8320 2.2068  13141 13390 13272.9340  40.7183  19.3695 0.6761  21.3689 0.6760
//  50119   33  46 37.7060 2.1207  16548 16859 16706.7250  48.5113  19.8277 0.6361  21.8273 0.6361
//  63096   33  48 38.8280 2.1271  20861 21174 21034.7400  53.5762  20.2796 0.6390  22.2793 0.6390
//  79433   34  48 39.6850 2.1637  26272 26655 26474.7520  57.8774  20.7219 0.6542  22.7216 0.6541
// 100000   36  50 40.5690 2.1409  33125 33525 33336.8960  66.8484  21.1564 0.6184  23.1562 0.6184
// ------  --- --- ------- ------  ----- ----- ---------- --------  ------- ------  ------- ------
// done in 153.5 seconds

//
// main program
//

int main(int argc,char **argv)
{
  int details = (argc == 3 && argv[1][0] == '-' && argv[1][1] == 'a' && atoi(argv[2]) > 0) ? 1 : 0;
  int n_experiments = 1000; // TO DO: use more (1000000 should take 2 to 3 hours)

  srandom(1u); // ensure reproducible results
  printf("                                  data for %d random trees\n",n_experiments);
  printf("          maximum tree height           number of leaves          calls on hit    calls on miss\n");
  printf("        ----------------------  -------------------------------  --------------  --------------\n");
  printf("     n  min max    mean    std    min   max       mean      std     mean    std     mean    std\n");
  printf("------  --- --- ------- ------  ----- ----- ---------- --------  ------- ------  ------- ------\n");
  for(int n_log = 1 * 10;n_log <= 4 * 10;n_log++)
  {
    int n = (int)round(pow(10.0,(double)n_log / 10.0)); // the number of nodes of the tree
    int a[n];                                           // the nodes' data
    tree_node *root,nodes[n];                           // the root and the storage space for the nodes of the tree
    int h_height[n + 1];                                // for an histogram of the heights of the random trees
    int h_leaves[n + 1];                                // for an histogram of the number of leaves of the random trees
    double mean,std;                                    // for mean and standard deviation computations
    double x,hit[2],miss[2];                            // for the average number of hits and misses
    int m,M;                                            // location of minima and maxima

    printf("%6d",n);
    //
    // the example in the slides
    //
    if(n == 10)
    {
      root = NULL;
      nodes[0].data = 3l; insert_node(&root,&nodes[0]);
      nodes[1].data = 1l; insert_node(&root,&nodes[1]);
      nodes[2].data = 9l; insert_node(&root,&nodes[2]);
      nodes[3].data = 7l; insert_node(&root,&nodes[3]);
      nodes[4].data = 5l; insert_node(&root,&nodes[4]);
      if(count_leaves(root) != 2)
      {
        fprintf(stderr,"count_leaves() returned a wrong value\n");
        exit(1);
      }
      if(tree_height(root) != 4)
      {
        fprintf(stderr,"tree_height() returned a wrong value\n");
        exit(1);
      }
      search_counter = 0;
      for(int i = 1;i <= 9;i += 2)
        if(search_tree(root,(long)i) == NULL)
          return 1; // impossible if the program is correct
      if(count_function_calls_on_hit(root,0) != search_counter)
      {
        fprintf(stderr,"count_function_calls_of_hit() returned a wrong value\n");
        exit(1);
      }
      search_counter = 0;
      for(int i = 0;i <= 10;i += 2)
        if(search_tree(root,(long)i) != NULL)
          return 1; // impossible if the program is correct
      if(count_function_calls_on_miss(root,0) != search_counter)
      {
        fprintf(stderr,"count_function_calls_of_miss() returned a wrong value\n");
        exit(1);
      }
    }
    //
    // the experiments
    //
    for(int i = 0;i <= n;i++)
      h_height[i] = h_leaves[i] = 0;
    hit[0] = hit[1] = miss[0] = miss[1] = 0.0;
    for(int n_experiment = 0;n_experiment < n_experiments;n_experiment++)
    {
      rand_perm(n,&a[0]);
      root = NULL;
      for(int i = 0;i < n;i++)
      {
        nodes[i].data = (long)a[i];
        insert_node(&root,&nodes[i]);
      }
      h_height[tree_height(root)]++;
      h_leaves[count_leaves(root)]++;
      x = (double)count_function_calls_on_hit(root,0) / (double)n; // there are n nodes
      hit[0] += x;
      hit[1] += x * x;
      x = (double)count_function_calls_on_miss(root,0) / (double)(n + 1); // there are n+1 NULL pointers
      miss[0] += x;
      miss[1] += x * x;
    }
    //
    // output summary
    //
    mean = std = 0.0;
    m = n + 1;
    M = -1;
    for(int i = 0;i <= n;i++)
      if(h_height[i] != 0)
      {
        mean += (double)i * (double)h_height[i];
        std += (double)i * (double)i * (double)h_height[i];
        if(i < m) m = i;
        if(i > M) M = i;
      }
    mean /= (double)n_experiments;
    std /= (double)n_experiments;
    std = sqrt(std - mean * mean);
    printf("  %3d %3d %7.4f %6.4f",m,M,mean,std);
    mean = std = 0.0;
    m = n + 1;
    M = -1;
    for(int i = 0;i <= n;i++)
      if(h_leaves[i] != 0)
      {
        mean += (double)i * (double)h_leaves[i];
        std += (double)i * (double)i * (double)h_leaves[i];
        if(i < m) m = i;
        if(i > M) M = i;
      }
    mean /= (double)n_experiments;
    std /= (double)n_experiments;
    std = sqrt(std - mean * mean);
    printf("  %5d %5d %10.4f %8.4f",m,M,mean,std);
    mean = hit[0] / (double)n_experiments;
    std = hit[1] / (double)n_experiments;
    std = sqrt(std - mean * mean);
    printf("  %7.4f %6.4f",mean,std);
    mean = miss[0] / (double)n_experiments;
    std = miss[1] / (double)n_experiments;
    std = sqrt(std - mean * mean);
    printf("  %7.4f %6.4f",mean,std);
    printf("\n");
    //
    // output the tree height data
    //
    if(details != 0 || n == atoi(argv[2]))
    {
      printf("      i frac height\n");
      printf("  ----- -----------\n");
      for(int i = 0;i <= n;i++)
        if(h_height[i] != 0)
          printf("  %5d %11.9f\n",i,(double)h_height[i] / (double)n_experiments);
      printf("  ----- -----------\n");
    }
    //
    // output the number of leaves data
    //
    if(details != 0 || n == atoi(argv[2]))
    {
      printf("      i frac leaves\n");
      printf("  ----- -----------\n");
      for(int i = 0;i <= n;i++)
        if(h_leaves[i] != 0)
          printf("  %5d %11.9f\n",i,(double)h_leaves[i] / (double)n_experiments);
      printf("  ----- -----------\n");
    }
    //
    // done
    //
    fflush(stdout);
  }
  printf("------  --- --- ------- ------  ----- ----- ---------- --------  ------- ------  ------- ------\n");
  printf("done in %.1f seconds\n",elapsed_time());
  return 0;
}
