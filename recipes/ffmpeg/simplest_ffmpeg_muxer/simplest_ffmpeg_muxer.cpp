﻿/**
 *
 * 最简单的基于FFmpeg的视音频复用器
 * Simplest FFmpeg Muxer

 * 雷霄骅 Lei Xiaohua
 * leixiaohua1020@126.com
 * 中国传媒大学/数字电视技术
 * Communication University of China / Digital TV Technology
 * http://blog.csdn.net/leixiaohua1020

 * 本程序可以将视频码流和音频码流打包到一种封装格式中。
 * 程序中将AAC编码的音频码流和H.264编码的视频码流打包成MP4封装格式的文件。
 * 需要注意的是本程序并不改变视音频的编码格式。

 * This software mux a video bitstream and a audio bitstream 
 * together into a file.
 * In this example, it mux a H.264 bitstream (in MPEG2TS) and 
 * a AAC bitstream file together into MP4 format file.
 *
 */

#include <stdio.h>

#define __STDC_CONSTANT_MACROS

extern "C"
{
#include <libavformat/avformat.h>
};


int main(int argc, char* argv[])
{
	int ret, i;
	int frame_index = 0;
	int videoindex_v = -1, videoindex_out = -1;
	int audioindex_a = -1, audioindex_out = -1;
	int64_t cur_pts_v = 0, cur_pts_a = 0;
	AVPacket pkt;
	AVFormatContext *ifmt_ctx_v = NULL, *ifmt_ctx_a = NULL, *ofmt_ctx = NULL;

	const char *in_filename_v = "cuc_ieschool.ts";	// Input file URL
	//const char *in_filename_v = "cuc_ieschool.h264";
	//const char *in_filename_a = "cuc_ieschool.mp3";
	//const char *in_filename_a = "gowest.m4a";
	//const char *in_filename_a = "gowest.aac";
	const char *in_filename_a = "huoyuanjia.mp3";
	const char *out_filename = "cuc_ieschool.mp4";	// Output file URL
	
	av_register_all();

	// Input
	if ((ret = avformat_open_input(&ifmt_ctx_v, in_filename_v, 0, 0)) < 0) 
	{
		printf( "Could not open input file.");
		goto end;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx_v, 0)) < 0) 
	{
		printf( "Failed to retrieve input stream information");
		goto end;
	}

	if ((ret = avformat_open_input(&ifmt_ctx_a, in_filename_a, 0, 0)) < 0) 
	{
		printf( "Could not open input file.");
		goto end;
	}

	if ((ret = avformat_find_stream_info(ifmt_ctx_a, 0)) < 0) 
	{
		printf( "Failed to retrieve input stream information");
		goto end;
	}
	
	printf("===========Input Information==========\n");
	av_dump_format(ifmt_ctx_v, 0, in_filename_v, 0);
	av_dump_format(ifmt_ctx_a, 0, in_filename_a, 0);
	printf("======================================\n");
	
	// Output
	ret = avformat_alloc_output_context2(&ofmt_ctx, NULL, NULL, out_filename);
	if (!ofmt_ctx) 
	{
		printf( "Could not create output context\n");
		goto end;
	}

	for (i = 0; i < ifmt_ctx_v->nb_streams; i++) 
	{
		// Create output AVStream according to input AVStream
		if (ifmt_ctx_v->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO)
		{
			AVStream *in_stream = ifmt_ctx_v->streams[i];
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

			videoindex_v = i;
			videoindex_out = out_stream->index;

			break;
		}
	}

	for (i = 0; i < ifmt_ctx_a->nb_streams; i++) 
	{
		// Create output AVStream according to input AVStream
		if (ifmt_ctx_a->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO)
		{
			AVStream *in_stream = ifmt_ctx_a->streams[i];
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

			audioindex_a = i;
			audioindex_out = out_stream->index;

			break;
		}
	}

	printf("==========Output Information==========\n");
	av_dump_format(ofmt_ctx, 0, out_filename, 1);
	printf("======================================\n");
	
	// Open output file
	if (!(ofmt_ctx->oformat->flags & AVFMT_NOFILE)) 
	{
		if (avio_open(&ofmt_ctx->pb, out_filename, AVIO_FLAG_WRITE) < 0) 
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
		int stream_index = 0;
		AVFormatContext *ifmt_ctx;
		AVStream *in_stream, *out_stream;

		// Get an AVPacket
		if (av_compare_ts(cur_pts_v, ifmt_ctx_v->streams[videoindex_v]->time_base, cur_pts_a, ifmt_ctx_a->streams[audioindex_a]->time_base) <= 0)
		{
			ifmt_ctx = ifmt_ctx_v;
			stream_index = videoindex_out;
			// 读视频 AVPacket
			if (av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do 
				{
					in_stream  = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if (pkt.stream_index == videoindex_v)
					{
						// FIX??No PTS (Example: Raw H.264)
						// Simple Write PTS
						if (pkt.pts == AV_NOPTS_VALUE)
						{
							// Write PTS
							AVRational time_base1 = in_stream->time_base;
							// Duration between 2 frames (us)
							int64_t calc_duration = (double)AV_TIME_BASE/av_q2d(in_stream->r_frame_rate);
							// Parameters
							pkt.pts = (double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							pkt.dts = pkt.pts;
							pkt.duration = (double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							frame_index++;
						}
						cur_pts_v = pkt.pts;
						break;
					}
				} while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			} 
			else
			{
				break;
			}
		} 
		else
		{
			ifmt_ctx = ifmt_ctx_a;
			stream_index = audioindex_out;
			// 读音频 AVPacket
			if (av_read_frame(ifmt_ctx, &pkt) >= 0)
			{
				do
				{
					in_stream  = ifmt_ctx->streams[pkt.stream_index];
					out_stream = ofmt_ctx->streams[stream_index];

					if (pkt.stream_index == audioindex_a)
					{
						// FIX??No PTS
						// Simple Write PTS
						if(pkt.pts == AV_NOPTS_VALUE)
						{
							// Write PTS
							AVRational time_base1 = in_stream->time_base;
							// Duration between 2 frames (us)
							int64_t calc_duration = (double)AV_TIME_BASE/av_q2d(in_stream->r_frame_rate);
							// Parameters
							pkt.pts = (double)(frame_index*calc_duration)/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							pkt.dts = pkt.pts;
							pkt.duration = (double)calc_duration/(double)(av_q2d(time_base1)*AV_TIME_BASE);
							frame_index++;
						}
						cur_pts_a = pkt.pts;
						break;
					}
				} while(av_read_frame(ifmt_ctx, &pkt) >= 0);
			}
			else
			{
				break;
			}
		}

		// Convert PTS/DTS
		pkt.pts = av_rescale_q_rnd(pkt.pts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.dts = av_rescale_q_rnd(pkt.dts, in_stream->time_base, out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
		pkt.duration = av_rescale_q(pkt.duration, in_stream->time_base, out_stream->time_base);
		pkt.pos = -1;
		pkt.stream_index = stream_index;

		printf("Write 1 Packet. size:%5d\tpts:%lld\n", pkt.size, pkt.pts);
		// Write
		if (av_interleaved_write_frame(ofmt_ctx, &pkt) < 0) 
		{
			printf( "Error muxing packet\n");
			break;
		}
		av_packet_unref(&pkt);

	}
	// Write file trailer
	av_write_trailer(ofmt_ctx);

end:
	// close input
	avformat_close_input(&ifmt_ctx_v);
	avformat_close_input(&ifmt_ctx_a);
	
	// close output
	if (ofmt_ctx && !(ofmt_ctx->oformat->flags & AVFMT_NOFILE))
		avio_close(ofmt_ctx->pb);
	avformat_free_context(ofmt_ctx);
	if (ret < 0 && ret != AVERROR_EOF) 
	{
		printf( "Error occurred.\n");
		return -1;
	}
	
	return 0;
}
