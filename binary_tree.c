//
// Tomás Oliveira e Silva, AED,  November 2018
//
// TO DO: Place your own identification here
//
// empirical study of random ordered binary trees
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
  // TO DO: delete the next line of code and place your code here
  // if link != null, e height corresponder ao nivel dado, esta top, cont++
  return search_counter;
}

static int count_function_calls_on_miss(tree_node *link,int level)
{
  // TO DO: delete the next line of code and place your code here
  return search_counter;
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
