/****************************************
 * souce file for GRAPH based challenge from TEJAS Net.
 *
 * Author : Amir Pate amir.patel76@gmail.com
 *
 * *************************************/
#include <stdio.h>

#include "test.h"

my_file_t *my_file; // global file DS
node_t *node_1; // 
static sigset_t   signal_mask;  // signals to block      

void my_handler1(int sig)
{
    // this will basically update the database from file
    printf("thread 1 got signal %d  \n",sig); 
    // same as handler 2

}

void my_handler2(int sig)
{
    int n;
    char ch;

    printf("thread 2 got signal %d  \n",sig); 

    pthread_mutex_lock(&my_file->flock);
    fseek(my_file->fptr, 0, SEEK_SET);
    fscanf(my_file->fptr, "%d", &n);
    n--;
    fseek(my_file->fptr, 0, SEEK_SET);
    fprintf(my_file->fptr,"%d",n);
    fscanf(my_file->fptr, "%c", &ch); 
    unsigned long x = ftell(my_file->fptr);
    fscanf(my_file->fptr, "%d", &n);
    fscanf(my_file->fptr, "%c", &ch);
   
    /* Incomplete... from here on wards deleting data related to vertex;*/
    pthread_mutex_unlock(&my_file->flock);
}

void my_handler3(int sig)
{
    printf("thread 3 got signal %d  \n",sig); 
}

void my_handler4(int sig)
{
    printf("thread 4 got signal %d  \n",sig); 
}




void* my_func(void *data)
{
    int n, i, n1, n2, j, flag, count, n3,n4;
    char ch, ch2;
    gnode_t *gnode = (gnode_t *)data;
    FILE * temp ; 
    int n_edge;
    graph_t *graph;

    // atexit(my_handler1);  // handler at exit of thread

    if(gnode->vertex == 0)
        signal(SIGUSR1,my_handler1);
    else if(gnode->vertex == 1)
        signal(SIGUSR1,my_handler2);
    else if(gnode->vertex == 2)
        signal(SIGUSR1,my_handler3);
    else if(gnode->vertex == 3)
        signal(SIGUSR1,my_handler4);
#if 0
    struct sigaction my_action;
    my_action.sa_handler = my_handler1;
    my_action.sa_flags = SA_RESTART;
    sigaction(SIGTERM, &my_action, NULL);
#endif
    //printf("dest=%dwt=%d \n",node->dest, node->wt); 
    //printf("%d %d ",gnode->node[0]->dest,gnode->node[0]->wt);
    pthread_mutex_lock(&my_file->flock);
    //
    // update the vertex count in file
    fseek(my_file->fptr, 0, SEEK_SET);
    fscanf(my_file->fptr, "%d", &n); 

    fseek(my_file->fptr, 0, SEEK_SET);
    n++;

    // update the edges count in file
    fprintf(my_file->fptr,"%d",n);
    fscanf(my_file->fptr, "%c", &ch); 
    unsigned long x = ftell(my_file->fptr);
    fscanf(my_file->fptr, "%d", &n); 
    fseek(my_file->fptr, x, SEEK_SET);
    // n += gnode->nedges;
    fprintf(my_file->fptr,"%d",n);
    n_edge = n;

    fscanf(my_file->fptr, "%c", &ch);  // skip '\n'

    // make your presence in file
     
    for(i = 1; i < gnode->vertex; i++)
    {
        fscanf(my_file->fptr, "%d", &n); 
        fscanf(my_file->fptr, "%c", &ch); 
    }

    fprintf(my_file->fptr,"%d",gnode->vertex);

    if (n_edge == 0) // if edges are 0 curently, you are the first to write to file
    {
        fseek(my_file->fptr, 0, SEEK_END);
        for (i = 0; i < gnode->nedges; i++)
        {
            fprintf(my_file->fptr,"%d%c%d%c%d%c",gnode->vertex,' ',gnode->node[i]->dest,' ',gnode->node[i]->wt,'\n');

            fseek(my_file->fptr, x, SEEK_SET); // update edge count
            fprintf(my_file->fptr,"%d",gnode->nedges);
            fseek(my_file->fptr, 0, SEEK_END); // update edge count
        }
    }
    else
    {
        while((ch = fgetc(my_file->fptr)) != '\n')
            ;
        count = 0;

        for (i = 0; i < gnode->nedges; i++)
        {
                flag = 0;

                unsigned long y = ftell(my_file->fptr);
                for (j = 0 ; j < n_edge; j++)
                {
                    fscanf(my_file->fptr, "%d%c%d%c%d%c",&n1,&ch,&n2,&ch,&n,&ch);
                    //printf("n1 = %d n2 = %d dest = %d ver = %d \n",n1,n2,gnode->node[i]->dest,gnode->vertex);
                    if (gnode->node[i]->dest == gnode->vertex ||
                            (n1 == gnode->node[i]->dest && n2 == gnode->vertex) || 
                            (n2 == gnode->node[i]->dest && n1 == gnode->vertex))
                    {
                        flag = 0;
                        //printf("%d \n",__LINE__);
                        break;
                    }
                    //printf("%d \n",__LINE__);
                    flag = 1;
                }
                if(flag)
                {
                    //printf("%d \n",__LINE__);
                    count++;
                    fseek(my_file->fptr, 0, SEEK_END);
                    fprintf(my_file->fptr,"%d%c%d%c%d%c",gnode->vertex,' ',gnode->node[i]->dest,' ',gnode->node[i]->wt,'\n');
                }
                fseek(my_file->fptr, y, SEEK_SET); // update edge count
            
            fseek(my_file->fptr, x, SEEK_SET); // update edge count
            fprintf(my_file->fptr,"%d",n_edge + count);
            fseek(my_file->fptr, 0, SEEK_END); // update edge count
            
        }
    }

    pthread_mutex_unlock(&my_file->flock);
    static int m = 0;
    graph = create_graph(3);
    while(1)
    {
        sleep(5);
        pthread_mutex_lock(&my_file->flock);
        fseek(my_file->fptr, 0, SEEK_SET);
        fscanf(my_file->fptr,"%d%c%d%c",&n1,&ch,&n2,&ch2);
        fscanf(my_file->fptr,"%d%c%d%c%d%c%d%c",&n1,&ch,&n4,&ch2,&n,&ch,&n3,&ch);

        for (i = 0; i < n2; i++)
        {
            fscanf(my_file->fptr,"%d%c%d%c%d%c",&n1,&ch,&n,&ch2,&n3,&ch);
            //printf("n1 = %d, n2 = %d n3 = %d \n",n1,n,n3);
            addedge(graph, n1, n,n3);
        }
        fseek(my_file->fptr, 0, SEEK_END); 
        pthread_mutex_unlock(&my_file->flock);
        printf("list for thered %d \n",gnode->vertex);
        print(graph);
    }

    return 0;
}

void prim_mst(int arr[][5])
{
    int n, n1, n2, n4, n3, i,j;
    char ch, ch2, ch3;
   pthread_mutex_lock(&my_file->flock);
   fseek(my_file->fptr, 0, SEEK_SET);
   fscanf(my_file->fptr,"%d%c%d%c",&n1,&ch,&n2,&ch2);
   printf("n1 = %d, n2 = %d\n",n1,n2);
   graph_t *graph = create_graph(n1);
   for (i = 0; i < n1; i++)
   {
       for (j = 0; j < n1 ; j++)
           arr[i][j] = 0;
   }

   fscanf(my_file->fptr,"%d%c%d%c%d%c%d%c",&n1,&ch,&n4,&ch2,&n,&ch,&n3,&ch);
    
   for (i = 0; i < n2; i++)
   {
       fscanf(my_file->fptr,"%d%c%d%c%d%c",&n1,&ch,&n,&ch2,&n3,&ch);
       arr[n1][n] = n3;
       printf("n1 = %d, n2 = %d n3 =%d \n",n1,n,n3);
   }

   for (i = 0; i < n1; i++)
   {
       for (j = 0; j < n1 ; j++)
           printf("%d ",arr[i][j]);
       puts("");
   }
   pthread_mutex_unlock(&my_file->flock);
   fseek(my_file->fptr, 0, SEEK_END); // update edge count
}
int main(){

   int n, i, j, e, w,n2,n3,n1,no_thread,n4;
   pid_t pid;
   char ch,ch2;
   int  ret;
   pthread_t thread_id,thread_id2, thread_id3, thread_id4, thread_sig_h;

   my_file = malloc(sizeof(my_file_t));
   my_file->fptr = fopen("graph_data2","r+");

#if 0
   printf("How many threds you want to create ??");
   scanf("%d",&no_thread);
   pthread_t id[no_thread];
#endif


   ret = fscanf(my_file->fptr, "%d", &n);
   if(ret < 0)
   {
       fprintf(my_file->fptr,"%d",0);
       //fprintf(my_file->fptr,"%c",' ');
       fputc(' ',my_file->fptr);
       fprintf(my_file->fptr,"%d",0);
       //fprintf(my_file->fptr,"%c",'\n');
       fputc('\n',my_file->fptr);
       for (i = 0; i < 4; i ++)
       {
           fprintf(my_file->fptr,"%d",0);
           fputc(' ',my_file->fptr);
       }
       fputc('\n',my_file->fptr);
   }

   if(pthread_mutex_init(&my_file->flock, NULL))
   {   
       perror("mutex init failure");
       return FAILURE;
   }   
   int nedges = 3;

   gnode_t *gnode = malloc(sizeof(gnode_t));
   gnode->vertex = 0;
   gnode->nedges = nedges;
   gnode->node = malloc(nedges * sizeof(node_t *));

   gnode->node[0] = malloc(sizeof (node_t));
   gnode->node[0]->dest = 1;
   gnode->node[0]->wt = 4;

   gnode->node[1] = malloc(sizeof (node_t));
   gnode->node[1]->dest = 2;
   gnode->node[1]->wt = 5;

   gnode->node[2] = malloc(sizeof (node_t));
   gnode->node[2]->dest = 0;
   gnode->node[2]->wt = 6;


   gnode_t *gnode2 = malloc(sizeof(gnode_t));
   gnode2->vertex = 1;
   gnode2->nedges = nedges;
   gnode2->node = malloc(nedges * sizeof(node_t *));

   gnode2->node[0] = malloc(sizeof (node_t));
   gnode2->node[0]->dest = 1;
   gnode2->node[0]->wt = 4;

   gnode2->node[1] = malloc(sizeof (node_t));
   gnode2->node[1]->dest = 2;
   gnode2->node[1]->wt = 5;

   gnode2->node[2] = malloc(sizeof (node_t));
   gnode2->node[2]->dest = 0;
   gnode2->node[2]->wt = 6;



#if 1
   gnode_t *gnode3 = malloc(sizeof(gnode_t));
   gnode3->vertex = 2;
   gnode3->nedges = nedges;
   gnode3->node = malloc(nedges * sizeof(node_t *));

   gnode3->node[0] = malloc(sizeof (node_t));
   gnode3->node[0]->dest = 2;
   gnode3->node[0]->wt = 4;

   gnode3->node[1] = malloc(sizeof (node_t));
   gnode3->node[1]->dest = 1;
   gnode3->node[1]->wt = 5;

   gnode3->node[2] = malloc(sizeof (node_t));
   gnode3->node[2]->dest = 0;
   gnode3->node[2]->wt = 6;
#endif

   // signal handling for multithreading 
#if 0
   sigemptyset (&signal_mask);
   sigaddset (&signal_mask, SIGINT);
   sigaddset (&signal_mask, SIGTERM);

   ret = pthread_sigmask (SIG_BLOCK, &signal_mask, NULL);
   if (ret != 0) {
       perror("sigmask");
       return -1;
   }

   ret = pthread_create (&sig_thr_id, NULL, signal_thread, NULL);
   if (ret != 0) {
       perror("thread creation error");
       return -1;
    }
#endif

   ret = pthread_create(&thread_id, NULL, my_func,gnode);
   ret = pthread_create(&thread_id2, NULL, my_func,gnode2);
   ret = pthread_create(&thread_id3, NULL, my_func,gnode3);
   //ret = pthread_create(&thread_id4, NULL, my_func,gnode);

   pthread_join(thread_id,NULL);
   pthread_join(thread_id2,NULL);
   pthread_join(thread_id3,NULL);
    
   // PRIM's MST algo in progress
#if 1
   int arr[5][5]; 
   prim_mst(arr);
#endif
   for (j = 0; j < 3; j++)
   {
       free(gnode->node[j]);
       free(gnode2->node[j]);
       free(gnode3->node[j]);
   }
   free(gnode->node);
   free(gnode);
   free(gnode2->node);
   free(gnode2);
   free(gnode3->node);
   free(gnode3);

   fclose(my_file->fptr);

   free(my_file);
   //pthread_join(thread_id4,NULL);
   

#if 0
   pthread_mutex_lock(&my_file->flock);
   fseek(my_file->fptr, 0, SEEK_SET);
   fscanf(my_file->fptr,"%d%c%d%c",&n1,&ch,&n2,&ch2);
   printf("n1 = %d, n2 = %d\n",n1,n2);
   graph_t *graph = create_graph(n1);

   fscanf(my_file->fptr,"%d%c%d%c%d%c%d%c",&n1,&ch,&n4,&ch2,&n,&ch,&n3,&ch);
    
   for (i = 0; i < n2; i++)
   {
       fscanf(my_file->fptr,"%d%c%d%c%d%c",&n1,&ch,&n,&ch2,&n3,&ch);
       printf("n1 = %d, n2 = %d n3 =%d \n",n1,n,n3);
       addedge(graph, n1, n,n3);
   }
   pthread_mutex_unlock(&my_file->flock);
   fseek(my_file->fptr, 0, SEEK_END); // update edge count
   print(graph);
#endif
#if 0
   addedge(graph, 0, 1,3);
    addedge(graph, 0, 4,5);
    addedge(graph, 1, 2,2);
    addedge(graph, 1, 3,7);
    addedge(graph, 1, 4, 9);
    addedge(graph, 2, 3, 10);
    addedge(graph, 3, 4, 14);
#endif
   fclose(my_file->fptr);
}





















