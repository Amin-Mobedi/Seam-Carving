#include "c_img.h"
#include "seamcarving.h"
#include <stdio.h>
#include <stdlib.h>

#include <math.h>


uint8_t get_delta(struct rgb_img *im,int i,int j){
    int H = im->height;
    int W = im->width;


    int R_X = get_pixel(im,j,(i+1)%(W),0) - get_pixel(im,j,i-1,0);
    int R_Y = get_pixel(im,(j+1)%(H),i,0) - get_pixel(im,j-1,i,0);
    int G_X = get_pixel(im,j,(i+1)%(W),1) - get_pixel(im,j,i-1,1);
    int G_Y = get_pixel(im,(j+1)%(H),i,1) - get_pixel(im,j-1,i,1);
    int B_X = get_pixel(im,j,(i+1)%(W),2) - get_pixel(im,j,i-1,2);
    int B_Y = get_pixel(im,(j+1)%(H),i,2) - get_pixel(im,j-1,i,2);

    if(i==0){
        R_X = get_pixel(im,j,(i+1)%(W),0) - get_pixel(im,j,W-1,0);
        G_X = get_pixel(im,j,(i+1)%(W),1) - get_pixel(im,j,W-1,1);
        B_X = get_pixel(im,j,(i+1)%(W),2) - get_pixel(im,j,W-1,2);
    }
    if(j==0){

        R_Y = get_pixel(im,(j+1)%(H),i,0) - get_pixel(im,H-1,i,0);
        G_Y = get_pixel(im,(j+1)%(H),i,1) - get_pixel(im,H-1,i,1);
        B_Y = get_pixel(im,(j+1)%(H),i,2) - get_pixel(im,H-1,i,2);


    }
    int energy = sqrt(R_X*R_X + R_Y*R_Y + G_X*G_X + G_Y*G_Y + B_X*B_X + B_Y*B_Y)/10;
    uint8_t fenergy = (uint8_t)energy;
    return fenergy;
}
void calc_energy(struct rgb_img *im, struct rgb_img **grad){

    int H = im->height;
    int W = im->width;
    create_img(grad, H, W);
    for(int i = 0; i<W;i++){
        for(int j = 0; j<H;j++){            
            int energy = get_delta(im,i,j); 
            set_pixel(*grad,j,i,energy,energy,energy);   
        }
    }
}

double calc_above_min(double* best_arr,int y, int x,int W,int H){
    double eng_min = 256;
    int i = -1;
    int j =1;
    if(x==0){
        i = 0;
    }
    if(x==(W-1)){
        j = 0;
    }
    while(i<=j){

    double p1 = (double) best_arr[(y-1)*W+x+i];

        if(p1 <= eng_min){
            eng_min = p1;
        }
    i++;

    }
    return eng_min;
    
    
}

void dynamic_seam(struct rgb_img *grad, double **best_arr){
    int H = grad->height;
    int W = grad->width;
    *best_arr = (double*)malloc(W*H*sizeof(double));
    for(int i = 0; i<W;i++){
 
        (*best_arr)[i] = (double) get_pixel(grad,0,i,1);
    }
    for(int y = 1; y<H;y++){
        for(int x =0;x<W;x++){
            (*best_arr)[(y*W)+x] = (double)get_pixel(grad, y, x, 1) + calc_above_min(*best_arr, y, x, W, H);
        }
    }

}
int calc_min(double* best_arr,int y, int x,int W,int H){
    double eng_min = best_arr[(y-1)*W+x];
    int i = -1;
    int j =1;
    int index;
    if(x==0){
        i = 0;
    }
    if(x==(W-1)){
        j = 0;
    }
    while(i<=j){

    double p1 = (double) best_arr[(y-1)*W+x+i];

        if(p1 <= eng_min){
            eng_min = p1;
            index = x+i;
        }
    i++;

    }
    return index;
    
    
}

void recover_path(double *best, int height, int width, int **path){
    *path = (int*)malloc(height*sizeof(int));

    int min_last_row = best[(height-1)*(width)];
    int min_index = 0;

    for(int x=0;x<width;x++){
        if(best[(height-1)*width+x]<min_last_row){
            min_last_row = best[(height-1)*width+x];
            min_index = x;
        }
    }
    (*path)[height-1] = min_index;
    for(int y = height-1;y >=1;y--){
        min_index = calc_min(best, y, min_index, width, height);
        (*path)[y-1] = min_index;
    }

    }

void remove_seam(struct rgb_img *src, struct rgb_img **dest, int *path){
    int width = src->width;
    int height = src->height;

    create_img(dest,height,width-1);
    for(int y = 0; y<(height);y++){
        for(int x = 0;x<(width-1);x++){
            if(x<path[y]){
                
                set_pixel(*dest,y,x,get_pixel(src,y,x,0),get_pixel(src,y,x,1),get_pixel(src,y,x,2));

            }
            else{
                
                set_pixel(*dest,y,x,get_pixel(src,y,x+1,0),get_pixel(src,y,x+1,1),get_pixel(src,y,x+1,2));

            }

        }
    }




}

// int main(){

//     struct rgb_img *im;
//     struct rgb_img *cur_im;
//     struct rgb_img *grad;
//     double *best;
//     int *path;

//     read_in_img(&im, "HJoceanSmall.bin");
    
//     for(int i = 0; i < 5; i++){
//         printf("i = %d\n", i);
//         calc_energy(im,  &grad);
//         dynamic_seam(grad, &best);
//         recover_path(best, grad->height, grad->width, &path);
//         remove_seam(im, &cur_im, path);

//         char filename[200];
//         sprintf(filename, "img2%d.bin", i);
//         write_img(cur_im, filename);


//         destroy_image(im);
//         destroy_image(grad);
//         free(best);
//         free(path);
//         im = cur_im;
//     }
//     destroy_image(im);


// }
