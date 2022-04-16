/**
 * 最简单的基于FFmpeg的封装格式转换器
 * Simplest FFmpeg Remuxer
 *
 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020
 *
 * 本程序实现了视频封装格式之间的转换。
 * 需要注意的是本程序并不改变视音频的编码格式。
 * 本程序修改自FFmpeg的tutorial。
 *
 * This software converts a media file from one container format
 * to another container format without encoding/decoding video files.
 * This software is modified from FFmpeg's tutorial.
 */

#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include <libavformat/avformat.h>
};


int main(int argc, char* argv[])
{
	int i, ret, frame_index = 0;
	const char *in_filename  = "cuc_ieschool1.flv";// Input file URL
	const char *out_filename = "cuc_ieschool1.mp4";// Output file URL
	AVPacket pkt;
	AVOutputFormat *ofmt = NULL;
	AVFormatContext *ifmt_ctx = NULL, *ofmt_ctx = NULL;

	av_register_all();
	
	// Input
	if ((ret = avformat_open_input(&ifmt_ctx, in_filename, 0, 0)) < 0) 
	{
		printf( "Could not open input file\n");
		goto end;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx, 0)) < 0) 
	{
		printf( "Failed to retrieve input stream information\n");
		goto end;
	}

	av_dump_format(ifmt_ctx, 0, in_filename, 0);
	
	// Output
	ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
	if (!ofmt_ctx) 
	{
		printf( "Could not create output context\n");
		goto end;
	}

	for (i = 0; i < ifmt_ctx->nb_streams; i++) 
	{
		// Create output AVStream according to input AVStream
		AVStream *in_stream = ifmt_ctx->streams[i];
		AVStream *out_stream = avformat_new_stream(ofmt_ctx, NULL);
		if (!out_stream) 
		{
			printf( "Failed allocating output stream\n");
			goto end;
		}

		// Copy the settings of AVCodecContext
		if (avcodec_parameters_copy(out_stream->codecpar, in_stream->codecpar) < 0) 
		{
			printf( "Failed to copy context from input to output stream codec context\n");
			goto end;
		}
		
		out_stream->codecpar->codec_tag = 0;
	}
	
	av_dump_format(ofmt_ctx, 0, out_filename, 1);
	
	// Open output file
	ofmt = ofmt_ctx->oformat;
	if (!(ofmt->flags & AVFMT_NOFILE)) 
	{
		ret = avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE);
		if (ret < 0) 
		{
			printf( "Could not open output file '%s'", out_filename);
			goto end;
		}
	}
	
	// Write file header
	if (avformat_write_header(ofmt_ctx, NULL) < 0) 
	{
		printf( "Error occurred when opening output file\n");
		goto end;
	}

	while (1) 
	{
		// Get an AVPacket
		ret = av_read_frame(ifmt_ctx, &pkt);
		if (ret < 0)
		{
			break;
		}

		AVStream *in_stream  = ifmt_ctx->streams[pkt.stream_index];
		AVStream *out_stream = ofmt_ctx->streams[pkt.stream_index];
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;

		// Write
		if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) 
		{
			printf( "Error muxing packet\n");
			break;
		}
		
		av_packet_unref(&pkt);
		frame_index++;
	}
	
	// Write file trailer
	av_write_trailer(ofmt_ctx);
	
	printf("Write %8d frames to output file\n", frame_index);
end:
	// close input
	avformat_close_input(&ifmt_ctx);
	
	// close output
	if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE))
		avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);
	
	return 0;
}
