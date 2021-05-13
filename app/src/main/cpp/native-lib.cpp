#include <jni.h>
#include <string>


extern "C" {
#include <libavformat/avformat.h>

}


extern "C" JNIEXPORT jstring JNICALL
Java_com_hqk_ndk_1audio_MainActivity_stringFromJNI(
        JNIEnv *env,
        jobject /* this */) {
    std::string hello = "Hello from C++";
    return env->NewStringUTF(hello.c_str());
}
extern "C"
JNIEXPORT void JNICALL
Java_com_hqk_ndk_1audio_MainActivity_playMusic(JNIEnv *env, jobject thiz, jstring input,
                                               jstring output) {


    const char *inputStr = env->GetStringUTFChars(input, NULL);
    const char *outputStr = env->GetStringUTFChars(output, NULL);

    //1、打开网络
    avformat_network_init();
    //2、媒体上下文
    AVFormatContext *avFormatContext = avformat_alloc_context();
    //3、打开文件
    if (avformat_open_input(&avFormatContext, inputStr, NULL, NULL) != 0) {
        return;
    }
    //4、获取音频文件信息，看能不能获取，如果不能，就终止
    if (avformat_find_stream_info(avFormatContext, NULL) < 0) {
        return;
    }
    //5、筛选音频流 为了记录下标
    int audioIndex = -1;
    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        if (avFormatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            //确定是音频
            audioIndex = i;
            break;
        }
    }
    // 解码器参数 目的为了拿到解码器ID
    AVCodecParameters *avCodecParameters = avFormatContext->streams[audioIndex]->codecpar;
    //6、拿到解码器 需要通过解码器ID，获取解码
    AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
    //7、解码器上下文
    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    //把参数设置到解码器上下文(关联起来)
    avcodec_parameters_to_context(avCodecContext, avCodecParameters);
    //8、打开解码器
    avcodec_open2(avCodecContext, avCodec, NULL);
    //9、拿到编码数据 压缩数据 AAC
    AVPacket *avPacket = av_packet_alloc(); // 打饭 拿 餐具
    //10、拿到原始数据
    while (av_read_frame(avFormatContext, avPacket) >= 0) {
        avcodec_send_packet(avCodecContext, avPacket);  //要打哪些菜
        AVFrame *avFrame = av_frame_alloc();//原始数据 最终的菜
        //11、取出 真正的原始数据
        int ret = avcodec_receive_frame(avCodecContext, avFrame);
        // avFrame 在这就有值了 音频 是对应的PCM原始数据
        // 拿到关键帧
        if (ret == AVERROR(EAGAIN)) {
            continue;//没有遇到关键帧，再次重试
        } else if (ret < 0) {
            return;//解码已经完成
        }
        //为了严谨性
        if (avPacket->stream_index != audioIndex) {
            continue;//遇到了非 音频，重试
        }
        //真正解码中。。。
        //声卡 要求 音频 输出的格式统一 （采用率统一，通道数统一，。。。）


    }


    env->ReleaseStringUTFChars(input, inputStr);
    env->ReleaseStringUTFChars(output, outputStr);
}