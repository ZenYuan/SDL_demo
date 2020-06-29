//
// Created by ZenYuan on 2020/6/26.
//

#include "common.h"

const char *file_path = "E:\\AV\\Mojito.mp4";

//解码出YUV数据
int decodec_test()
{
    AVFormatContext *pFormatContext = nullptr;
    //read file header of file format
    if(avformat_open_input(&pFormatContext, file_path, nullptr, nullptr) < 0){
        cout<<"avformat_open_input error\n"<<endl;
        return -1;
    }

    //get stream from format
    if(avformat_find_stream_info(pFormatContext, nullptr) < 0){
        cout<<"avformat_find_stream_info"<<endl;
        return -1;
    }

    int videoIndex = -1;
    for(int i = 0; i < pFormatContext->nb_streams; i++){
        if(pFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }
    if(-1 == videoIndex){
        cout<<"not a video stream\n"<<endl;
        return -1;
    }

    AVCodecParameters *pCodecParameters = pFormatContext->streams[videoIndex]->codecpar;
    //根据id值注册一个解码器。
    AVCodec *pCodec = avcodec_find_decoder(pCodecParameters->codec_id);
    //初始化解码器参数,将文件数据解码器参数拷贝
    AVCodecContext *pCodecContext = avcodec_alloc_context3(pCodec);
    if(nullptr == pCodecContext){
        cout<<"pCodecContext error\n"<<endl;
        return -1;
    }
    //解码器参数不可修改
    avcodec_parameters_to_context(pCodecContext, pCodecParameters);
    //打开编码器，此处预先解码头信息，但是没有帧数据
    if(avcodec_open2(pCodecContext, pCodec, nullptr) < 0){
        cout<<"avcodec_open2"<<endl;
        return -1;
    }

    cout<<"#####################file Infomation#############\n"<<endl;
    av_dump_format(pFormatContext, videoIndex, file_path, 0);
    cout<<"#####################end #######################\n"<<endl;


    AVFrame *pFrame = av_frame_alloc();
    AVFrame *pFrame_bst_YUV = av_frame_alloc();
    AVPacket *pPacket = static_cast<AVPacket*>(av_malloc(sizeof(AVPacket)));
    if(nullptr == pPacket){
        cout<<"av_malloc packet error\n"<<endl;
        return -1;
    }

    int dst_w = 1280;//pCodecContext->width;
    int dst_h = 720;//pCodecContext->height;

    //为即将转化后的帧分配足够的空间，该空间至少要能存放下原来的数据空间
    int numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, pCodecContext->width, pCodecContext->height, 1);
    uint8_t *out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFrame_bst_YUV->data, pFrame_bst_YUV->linesize, out_buffer, AV_PIX_FMT_RGB24, pCodecContext->width, pCodecContext->height, 1);

    //由于每一帧的头信息相同，在for循环外部进行初始化图像缩放or色彩空间转换初始化(创建视频格式转换句柄)
    SwsContext *img_swsContext = nullptr;
    img_swsContext = sws_getContext(pCodecContext->width, pCodecContext->height, pCodecContext->pix_fmt,    //源帧图片数据的高宽和编码图片原始格式(raw format)
                                    dst_w, dst_h, AV_PIX_FMT_RGB24,                                      //目标帧图片数据的高宽和编码图片原始格式(raw format)
                                    SWS_BICUBIC, nullptr, nullptr, nullptr);                               //颜色转换标识符，提高图片质量

    //创建YUV输出文件
    FILE* file_yuv = fopen("out.yuv", "wb+");
    if(nullptr == file_yuv){
        cout<<"fopen error\n"<<endl;
        return -1;
    }

    while(av_read_frame(pFormatContext, pPacket) >= 0){
        if(pPacket->stream_index == videoIndex){
            if(0 != avcodec_send_packet(pCodecContext, pPacket)){
                cout<<"send packet data to codecContext error\n"<<endl;
                return -1;
            }
            while(1){
                //pCodecContext中含有多个帧
                int res = avcodec_receive_frame(pCodecContext, pFrame);
                if(AVERROR(EAGAIN) == res || AVERROR_EOF == res){
                    break;
                }
                else if(res < 0){
                    cout<<"reveice a fream  from pCodecContext error\n"<<endl;
                    return -1;
                }
                if(res >= 0){
                    sws_scale(img_swsContext, const_cast<uint8_t const * const *>(pFrame->data), pFrame->linesize, 0, pCodecContext->height, pFrame_bst_YUV->data, pFrame_bst_YUV->linesize);
                    //planar数据分层存放，packed数据打包存放在data[0]
                    fwrite(pFrame_bst_YUV->data[0], 1, pCodecContext->height*pCodecContext->width * 3, file_yuv);
//                    fwrite(pFrame_bst_YUV->data[1], 1, pCodecContext->height*pCodecContext->width/4, file_yuv);
//                    fwrite(pFrame_bst_YUV->data[2], 1, pCodecContext->height*pCodecContext->width/4, file_yuv);
                }
            }
        }
    }
    fclose(file_yuv);
    sws_freeContext(img_swsContext);
    av_frame_free(&pFrame);
    av_frame_free(&pFrame_bst_YUV);
    //avcodec_free_context(&pCodecContext);
    avcodec_close(pCodecContext);
    avformat_close_input(&pFormatContext);
}

