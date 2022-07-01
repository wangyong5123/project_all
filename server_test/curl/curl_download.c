#include <stdio.h>

#include <curl/curl.h>

struct FtpFile {
  const char *filename;
  FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
  struct FtpFile *out = (struct FtpFile *)stream;
  if(out && !out->stream) {
    /* 打开文件以进行写操作 */ 
    out->stream = fopen(out->filename, "wb");
    if(!out->stream)
      return -1; /* failure, can't open file to write */ 
  }
  return fwrite(buffer, size, nmemb, out->stream);
}


int main(void)
{
  CURL *curl;
  CURLcode res;
  struct FtpFile ftpfile = {
    "curl.txt", /* 若FTP下载成功，名命下载后的文件为"curl.txt" */ 
    NULL
  };

  curl_global_init(CURL_GLOBAL_DEFAULT);

  curl = curl_easy_init();
  if(curl) {
    curl_easy_setopt(curl, CURLOPT_URL,
                     "ftp://192.168.3.126/a.txt");//下载指定的文件
    /* 定义回调函数，以便在需要写入数据时进行调用 */ 
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite);
    /*设置一个指向我们的结构的指针传递给回调函数*/ 
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile);

    /* 打开完整的协议/调试输出*/ 
    curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
    //char user_key[36];
    //sprintf(user_key,"%s\n","ftptest:ftptest");
    curl_easy_setopt(curl, CURLOPT_USERPWD, "ftpuser:ftpuser");

    res = curl_easy_perform(curl);

    /* 释放所有curl资源*/ 
    curl_easy_cleanup(curl);

    if(CURLE_OK != res) {
      /*容错处理 */ 
      fprintf(stderr, "curl told us %d\n", res);
    }
  }

  if(ftpfile.stream)
    fclose(ftpfile.stream); /* 关闭本地文件 */ 
 /*释放所有curl资源*/
  curl_global_cleanup();

  return 0;
}
