#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>
#include <errno.h>

int** pixel_map;
int height, width, threads_num, max_val;
pthread_t *threads;
int* hist;
pthread_mutex_t* hist_lock;
void (*hist_fun)(int);

void error(char* msg){
    perror(msg);
    exit(errno);
}

void read_file(char* filename){
    FILE* file = fopen(filename, "r");
    if(file == NULL) error("opening source file");
    if(fscanf(file,"P2 \n %d %d\n %d",&width, &height, &max_val) != 3) error("wrong file format");
    if(max_val > 255) error("wrong pixel values");
    hist = (int*)calloc(max_val+1, sizeof(int));
    hist_lock = (pthread_mutex_t*)calloc(max_val+1,sizeof(pthread_mutex_t));
    pixel_map = (int**)calloc(height,sizeof(int*));
    for (int i = 0; i < height; i++){
        pixel_map[i] = (int*)calloc(width,sizeof(int));
        for (int j = 0; j < width; j++){
            fscanf(file," %d",&pixel_map[i][j]);
        }
    }
    fclose(file);
}

void write_file(char* filename){
    FILE* file = fopen(filename, "w");
    if(file == NULL) error("opening result file");
    for (int i = 0; i < max_val+1; i++){
        fprintf(file,"%03d: %d\n",i,hist[i]);
    }
    fclose(file);
}

void* hist_wrapper(void* thread_no){
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    hist_fun((int)thread_no);
    gettimeofday(&stop, NULL);
    long int* time = malloc(sizeof(long int));
    *time = (long int)((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec);
    return (void*)time;
}

void hist_sign(int k){
    int max = max_val/threads_num*(k+1);
    int min = max_val/threads_num*k;
    int x;
    for(int i = 0; i < height ; i++)
        for(int j = 0; j < width; j++){
            x = pixel_map[i][j];
            if(x <= max && x > min) hist[x]++;
        }
}

void hist_block(int k){
    int x;
    int stop = (width + threads_num - 1)/threads_num*(k+1);
    int start = (width + threads_num - 1)/threads_num*k;
    for(int i = 0; i < height ; i++)
        for(int j = start; j < stop ; j++){
            x = pixel_map[i][j];
            pthread_mutex_lock(&hist_lock[x]);
            hist[x]++;
            pthread_mutex_unlock(&hist_lock[x]);
        }
}

void hist_interleaved(int k){
    int x;
    for(int j = 0; k + j < width ; j+=threads_num )
        for(int i = 0; i < height ; i++){
            x = pixel_map[i][k+j];
            pthread_mutex_lock(&hist_lock[x]);
            hist[x]++;
            pthread_mutex_unlock(&hist_lock[x]);
        }
}

void start_threads(){
    for(int i = 0; i < threads_num; i++){
        if((errno = pthread_create(&threads[i], NULL, hist_wrapper, (void*)i))!= 0) error("thread create");
    }
}

void collect_threads(){
    long int *result = malloc(sizeof(long int));
    for(int i = 0; i < threads_num; i++){
        if((errno = pthread_join(threads[i], (void**)&result)) != 0) error("thread join");
        printf("thread %ld, time: %ld\n", threads[i], *result);
    }
    free((void*)result);
}

void select_mode(char* mode){
    if(strcmp("sign", mode) == 0) hist_fun = hist_sign;
    else if(strcmp("block", mode) == 0) hist_fun = hist_block;
    else if(strcmp("interleaved", mode) == 0) hist_fun = hist_interleaved;
    else {
        errno = 22;
        error("wrong mode");
    }
}

int main(int argc, char** argv){
    if(argc != 5){
        errno = 22;
        error("wrong number of arguments");
    }
    if((threads_num = atoi(argv[1])) < 1){
        errno = 22;
        error("wrong number of threads");
    }
    threads = (pthread_t*)calloc(threads_num,sizeof(pthread_t));
    select_mode(argv[2]);
    read_file(argv[3]);
    
    struct timeval stop, start;
    gettimeofday(&start, NULL);
    
    start_threads();
    collect_threads();

    gettimeofday(&stop, NULL);
    long int time = (stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec;
    printf("total time: %ld\n",time);
    
    write_file(argv[4]);
    return 0;
}