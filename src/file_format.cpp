
#include <cstdio>
#include <cstdlib>
#include <cstring>
void format_file(const char srcFileName[],const char dstFileName[]){
  if(strcmp(srcFileName,dstFileName)==0){
    fprintf(stderr,"Error: src and dst file is same.\n");
    exit(-1);
  }
  FILE* srcFile=fopen(srcFileName,"r");
  FILE* dstFile=fopen(dstFileName,"w");
  while(true){
    int c=fgetc(srcFile);
    if(c==EOF){
      break;
    }
    else if(c>0&&c<128){
      if(c=='\t'){
        fputc(' ',dstFile);
      }
      else{
        fputc(c,dstFile);
      }
    }
  }  
  fclose(srcFile);
  fclose(dstFile);
}