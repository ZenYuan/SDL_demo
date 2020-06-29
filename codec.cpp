//
// Created by ZenYuan on 2020/6/26.
//
#include "common.h"

//const char* out_path = "codec_test.mp4";
//
//static int fill_stream_info(AVStream* avs, AVCodec** avc, AVCodecContext** avcc)
//{
//    *avc = avcodec_find_decoder(avs->codec->codec_id);
//    *avcc = avcodec_alloc_context3(*avc);
//    avcodec_parameters_to_context(*avcc, avs->codecpar);
//    avcodec_open2(*avcc, *avc, nullptr);
//    return 0;
//}
//
//static int prepare_decoder(AVFormatContext* avfc, AVCodec** avc, AVCodecContext** avcc)
//{
//    for(int i = 0; i < avfc->nb_streams; i++)
//    {
//        if(avfc->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO)
//        {
//            videoIndex = i;
//            fill_stream_info(avfc->streams[i], avc, avcc);
//        }
//    }
//    return 0;
//}
//
//int codec_test()
//{
//    int ret = 0;
//    AVCodecContext* avcc = nullptr;
//    AVCodec* avc = nullptr;
//
//    //获取视频流参数
//    AVFormatContext *avfc = nullptr;
//    avformat_open_input(&avfc, "E:\\AV\\Mojito.mp4", nullptr, nullptr);
//    avformat_find_stream_info(avfc, nullptr);
//    int videoIndex = -1;
//
//    //初始化解码器
//    prepare_decoder(avfc, &avc, &avcc);
//
//    //分配空间，存放编码后的数据
//    AVFormatContext *out_avfc = nullptr;
//    AVCodecContext *out_avcc = nullptr;
//    AVCodec *out_avc = nullptr;
//    AVStream *out_avs = nullptr;
//
//    ret = avformat_alloc_output_context2(&out_avfc, nullptr, nullptr, out_path);
//    if(ret < 0)
//    {
//        cout<<"avformat_alloc_output_context2 error\n"<<endl;
//        return -1;
//    }
//
//    out_avs = avformat_new_stream(out_avfc, nullptr);
//    avcodec_parameters_copy(out_avs->codecpar, avfc->streams[videoIndex]->codecpar);
//    avcodec_open2(out_avcc, out_avc, nullptr);
//    if (out_avfc->oformat->flags & AVFMT_GLOBALHEADER)
//    {
//        out_avfc->flags |= AV_CODEC_FLAG_GLOBAL_HEADER;
//    }
//
//
//
//    return 0;
//}

//该YUV数据是格式转化完成的，不能够进行转码
int code_test()
{
    AVFormatContext * pFormatCtx = nullptr;
    AVOutputFormat * fmt = nullptr;

    AVStream *video_s;
    AVCodecContext *pCodecCtx = nullptr;
    AVCodec *pCodec = nullptr;

    AVFrame *pFram = nullptr;

    FILE* in_file = fopen("E:\\AV\\out.yuv", "rb");
    int in_w = 1920;
    int in_h = 1080;
    const char* out_file = "out.h264";

    //为输出视频分配AVFormatContext空间
    //method1
    pFormatCtx = avformat_alloc_context();
    //guess output fmt
    fmt = av_guess_format(nullptr, out_file, nullptr);
    pFormatCtx->oformat = fmt;

#if 0
    //method2
    avformat_alloc_output_context2(&pFormatCtx, nullptr, nullptr, out_file);
    fmt = pFormatCtx->oformat;
#endif

    //打开输出视频
    if(avio_open(&pFormatCtx->pb, out_file, AVIO_FLAG_WRITE) < 0)
    {
        cout<<"avio_open error\n"<<endl;
        return -1;
    }

    //创建放在AVFormatContext中的AVStream
    //4.x以后codec参数被弃用
    video_s = avformat_new_stream(pFormatCtx, 0);
    if(nullptr == video_s)
    {
        cout<<"avformat_new_stream error\n"<<endl;
        return -1;
    }

    //设置编码参数
    pCodec = avcodec_find_encoder(fmt->video_codec);
    pCodecCtx = avcodec_alloc_context3(pCodec);

    pCodecCtx->codec_id = fmt->video_codec;
    pCodecCtx->codec_type = AVMEDIA_TYPE_VIDEO;
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    pCodecCtx->width = in_w;
    pCodecCtx->height = in_h;
    pCodecCtx->bit_rate = 400000;
    pCodecCtx->gop_size = 250;

    //每秒播放的图片张数
    pCodecCtx->time_base.num = 1;
    pCodecCtx->time_base.den = 25;

    //H264
    //最大最小量化器
    pCodecCtx->qmax = 51;
    pCodecCtx->qmin = 10;

    pCodecCtx->max_b_frames = 3;

    //初始化编码器文件描述符
    if(avcodec_open2(pCodecCtx, pCodec, nullptr) < 0)
    {
        cout<<"avcodec_open2 error\n"<<endl;
        return -1;
    }
    avcodec_parameters_from_context(video_s->codecpar, pCodecCtx);

    av_dump_format(pFormatCtx, 0, out_file, 1);

    //设置帧参数
    pFram = av_frame_alloc();
    int numBytes = av_image_get_buffer_size(pCodecCtx->pix_fmt, in_w, in_h, 1);
    uint8_t *out_buffer = (uint8_t *)av_malloc(numBytes * sizeof(uint8_t));
    av_image_fill_arrays(pFram->data, pFram->linesize, out_buffer, pCodecCtx->pix_fmt, in_w, in_h, 1);

    pFram->width = in_w;
    pFram->height = in_h;
    pFram->format = pCodecCtx->pix_fmt;

    //写头
    if(avformat_write_header(pFormatCtx, nullptr) < 0)
    {
        cout<<"avformat_write_header error\n"<<endl;
        return -1;
    }

    int i = 0;
    while(true)
    {
        if(fread(out_buffer, 1, in_w*in_h*3/2, in_file) <= 0)
        {
            break;
        }
        else if(feof(in_file))
        {
            break;
        }
        pFram->data[0] = out_buffer;
        pFram->data[1] = out_buffer + in_w * in_h;
        pFram->data[3] = out_buffer + in_w * in_h * 5/4;
        pFram->pts = i++;

        //将帧写入packet
        AVPacket *pPacket = av_packet_alloc();
        if(nullptr == pPacket)
        {
            cout<<"av_packet_alloc is error\n"<<endl;
            return -1;
        }
        int res = avcodec_send_frame(pCodecCtx, pFram);
        while(res >= 0)
        {
            res = avcodec_receive_packet(pCodecCtx, pPacket);
            if(res == AVERROR(EAGAIN) || res == AVERROR_EOF)
            {
                break;
            }
            else if(res < 0)
            {
                cout<<"avcodec_receive_packet error\n"<<endl;
                return -1;
            }
            pPacket->stream_index = video_s->index;
            //av_write_frame
            av_interleaved_write_frame(pFormatCtx, pPacket);
        }

        av_packet_unref(pPacket);
        av_packet_free(&pPacket);
    }
    av_write_trailer(pFormatCtx);

    avcodec_free_context(&pCodecCtx);
    pCodecCtx = nullptr;
    av_frame_free(&pFram);
    pFram = nullptr;

    return 0;
}
