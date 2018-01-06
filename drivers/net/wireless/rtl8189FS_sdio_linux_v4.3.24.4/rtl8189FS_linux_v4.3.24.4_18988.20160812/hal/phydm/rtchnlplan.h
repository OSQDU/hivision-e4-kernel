/******************************************************************************
 *
 * Copyright(c) 2007 - 2012 Realtek Corporation. All rights reserved.
 *                                        
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of version 2 of the GNU General Public License as
 * published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
 * FITNESS FOR A PARTICULAR PURPOSE. See the GNU General Public License for
 * more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
 *
 *
 ******************************************************************************/


#ifndef	__RT_CHANNELPLAN_H__
#define __RT_CHANNELPLAN_H__

typedef enum _RT_CHANNEL_DOMAIN_NEW
{

	//===== Add new channel plan above this line ===============//

	// For new architecture we define different 2G/5G CH area for all country.
	// 2.4 G only
	RT_CHANNEL_DOMAIN_2G_WORLD_5G_NULL				= 0x20,
	RT_CHANNEL_DOMAIN_2G_ETSI1_5G_NULL				= 0x21,
	RT_CHANNEL_DOMAIN_2G_FCC1_5G_NULL				= 0x22,
	RT_CHANNEL_DOMAIN_2G_MKK1_5G_NULL				= 0x23,
	RT_CHANNEL_DOMAIN_2G_ETSI2_5G_NULL				= 0x24,
	// 2.4 G + 5G type 1
	RT_CHANNEL_DOMAIN_2G_FCC1_5G_FCC1				= 0x25,
	RT_CHANNEL_DOMAIN_2G_WORLD_5G_ETSI1				= 0x26,
	//RT_CHANNEL_DOMAIN_2G_WORLD_5G_ETSI1				= 0x27,
	// .....

	RT_CHANNEL_DOMAIN_MAX_NEW,
	
}RT_CHANNEL_DOMAIN_NEW, *PRT_CHANNEL_DOMAIN_NEW;


#if 0
#define DOMAIN_CODE_2G_WORLD \
        {1,2,3,4,5,6,7,8,9,10,11,12,13}, 13
#define DOMAIN_CODE_2G_ETSI1 \
        {1,2,3,4,5,6,7,8,9,10,11,12,13}, 13
#define DOMAIN_CODE_2G_ETSI2 \
        {1,2,3,4,5,6,7,8,9,10,11}, 11
#define DOMAIN_CODE_2G_FCC1 \
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14}, 14
#define DOMAIN_CODE_2G_MKK1 \
        {10,11,12,13}, 4

#define DOMAIN_CODE_5G_ETSI1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140}, 19
#define DOMAIN_CODE_5G_ETSI2 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165}, 24
#define DOMAIN_CODE_5G_ETSI3 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,149,153,157,161,165}, 22
#define DOMAIN_CODE_5G_FCC1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165}, 24
#define DOMAIN_CODE_5G_FCC2 \
        {36,40,44,48,149,153,157,161,165}, 9
#define DOMAIN_CODE_5G_FCC3 \
        {36,40,44,48,52,56,60,64,149,153,157,161,165}, 13
#define DOMAIN_CODE_5G_FCC4 \
        {36,40,44,48,52,56,60,64,149,153,157,161}, 12
#define DOMAIN_CODE_5G_FCC5 \
        {149,153,157,161,165}, 5
#define DOMAIN_CODE_5G_FCC6 \
        {36,40,44,48,52,56,60,64}, 8
#define DOMAIN_CODE_5G_FCC7 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define DOMAIN_CODE_5G_IC1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define DOMAIN_CODE_5G_KCC1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,149,153,157,161,165}, 20
#define DOMAIN_CODE_5G_MKK1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140}, 19
#define DOMAIN_CODE_5G_MKK2 \
        {36,40,44,48,52,56,60,64}, 8
#define DOMAIN_CODE_5G_MKK3 \
        {100,104,108,112,116,120,124,128,132,136,140}, 11
#define DOMAIN_CODE_5G_NCC1 \
        {56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 24
#define DOMAIN_CODE_5G_NCC2 \
        {56,60,64,149,153,157,161,165}, 8  			
#define UNDEFINED \
        {0}, 0
#endif

//
//
//
/*

Countries							"Country Abbreviation"	Domain Code					SKU's	Ch# of 20MHz
															2G			5G						Ch# of 40MHz
"Albania焊ぺェㄈ"					AL													Local Test	
									    					        	            	
"Algeria焊のㄈ"					DZ									CE TCF	    	
					                    					        	            	
"Antigua & Barbuda矗ナ畄&ぺガ笷"	AG						2G_WORLD					FCC TCF	
					                    					        	        		
"Argentina"					AR						2G_WORLD					Local Test	
									    					        	        		
"Armeniaㄈェㄈ"					AM						2G_WORLD					ETSI	
									    					        	        		
"Aruba緗ぺ畄"						AW						2G_WORLD					FCC TCF	
															        	
"Australia緿瑆"						AU						2G_WORLD		5G_ETSI2		
															        	
"Austria而"						AT						2G_WORLD		5G_ETSI1	CE	
					    			    					        	
"Azerbaijan峨臼"				AZ						2G_WORLD					CE TCF	
					                    					        	
"Bahamasぺ皑"						BS						2G_WORLD				
									    					        	
"Barbadosぺぺ吹"					BB						2G_WORLD					FCC TCF	
									    					        	
"Belgiumゑ"						BE						2G_WORLD		5G_ETSI1	CE	
															        	
"Bermudaκ紏笷"						BM						2G_WORLD					FCC TCF	
															        	        		
"Brazilぺ﹁"						BR						2G_WORLD					Local Test	
									    					        	
"Bulgaria玂ㄈ"					BG						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Canada"						CA						2G_FCC1			5G_FCC7		IC / FCC	IC / FCC
					                    					        	
"Cayman Islands秨耙竤畄"			KY						2G_WORLD		5G_ETSI1	CE	
					                    					        	
"Chile醇"							CL						2G_WORLD					FCC TCF	
																    	
"Chinaい瓣"							CN						2G_WORLD		5G_FCC5		獺场?2002353? 	
					                    					        	
"Columbiaゑㄈ"					CO						2G_WORLD					Voluntary 	
					    			    					        	
"Costa Rica吹笷兢"				CR						2G_WORLD					FCC TCF	
					    			    					        	
"Cyprus峨隔吹"					CY						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Czech 倍"						CZ						2G_WORLD		5G_ETSI1	CE	
															        	
"Denmarkう沉"						DK						2G_WORLD		5G_ETSI1	CE	
					                    					        	
"Dominican Republicェ㎝瓣"	DO						2G_WORLD					FCC TCF	
					                    					        	
"Egypt甁の"	EG	2G_WORLD			CE T												CF	    	
					                    					        	
"El Salvador履焊ニ"				SV						2G_WORLD					Voluntary	
					        		    					        	
"Estonia稲‵ェㄈ"					EE						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Finland孽"						FI						2G_WORLD		5G_ETSI1	CE	
															        	
"France猭瓣"						FR										5G_E		TSI1	CE	
															        	
"Germany紈瓣"						DE						2G_WORLD		5G_ETSI1	CE	
															        	
"Greece 镁"						GR						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Guam闽畄"							GU						2G_WORLD				
									    					        	
"Guatemalaナ皑┰"					GT						2G_WORLD				
									    					        	
"Haiti"							HT						2G_WORLD					FCC TCF	
									    					        	
"HondurasЩ常┰吹"					HN						2G_WORLD					FCC TCF	
									    					        	
"Hungary"						HU						2G_WORLD		5G_ETSI1	CE	
															        	
"Iceland畄"						IS						2G_WORLD		5G_ETSI1	CE	
									    					        	
"India"							IN						2G_WORLD		5G_FCC3		FCC/CE TCF	
															        	
"Ireland稲焊孽"						IE						2G_WORLD		5G_ETSI1	CE	
															        	
"Israel︹"						IL										5G_F		CC6	CE TCF	
															        	
"Italy竡"						IT						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Japanらセ"							JP						2G_MKK1			5G_MKK1		MKK	MKK
																    	
"Korea龙瓣"							KR						2G_WORLD		5G_KCC1		KCC	KCC
									    					        	
"Latvia┰叉蝴ㄈ"					LV						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Lithuaniaミ吵﹞"					LT						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Luxembourg縞此躇"					LU						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Malaysia皑ㄓ﹁ㄈ"					MY						2G_WORLD					Local Test	
									    					        	
"Malta皑焊"						MT						2G_WORLD		5G_ETSI1	CE	
															        	
"Mexico茎﹁"						MX						2G_WORLD		5G_FCC3		Local Test	
															        	
"Morocco集"						MA													CE TCF	
									    					        	
"Netherlands颤孽"					NL						2G_WORLD		5G_ETSI1	CE	
									    					        	
"New Zealand﹁孽"					NZ						2G_WORLD		5G_ETSI2		
									    					        	
"Norway"						NO						2G_WORLD		5G_ETSI1	CE	
															        	
"Panamaぺ皑 "						PA						2G_FCC1						Voluntary	
									    					        	
"Philippines滇猾"					PH						2G_WORLD					FCC TCF	
									    					        	
"Poland猧孽"						PL						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Portugal覆靛"					PT						2G_WORLD		5G_ETSI1	CE	
									    					        	
"Romania霉皑ェㄈ"					RO						2G_WORLD		5G_ETSI1	CE	
					        		    					        	
"Russia玐霉吹"						RU						2G_WORLD		5G_ETSI3	CE TCF	
					        		    					        	
"Saudi Arabia‵┰"			SA						2G_WORLD					CE TCF	
					        		    					        	
"Singapore穝℡"					SG						2G_WORLD				
									    					        	
"Slovakia吹ワ"					SK						2G_WORLD		5G_ETSI1	CE	
					    			    					        	
"Slovenia吹蝴ェㄈ"				SI						2G_WORLD		5G_ETSI1	CE	
					    			    					        	
"South Africa玭獶"					ZA						2G_WORLD					CE TCF	
					        		    					        	
"Spain﹁痁"						ES										5G_ETSI1	CE	
															        	
"Sweden风ㄥ"						SE						2G_WORLD		5G_ETSI1	CE	
					        		    					        	
"Switzerland风"					CH						2G_WORLD		5G_ETSI1	CE	
					        		    					        	
"Taiwan籓芖"						TW						2G_FCC1			5G_NCC1	NCC	
															        	
"Thailand瓣"						TH						2G_WORLD					FCC/CE TCF	
															        	
"Turkeyφㄤ"						TR						2G_WORLD				
															        	
"Ukraine疩孽"						UA						2G_WORLD					Local Test	
					        		    					        	
"United Kingdom璣瓣"				GB						2G_WORLD		5G_ETSI1	CE	ETSI
					    			    					        	
"United States瓣"					US						2G_FCC1			5G_FCC7		FCC	FCC
					    			    					        	
"Venezuela〆ず风┰"					VE						2G_WORLD		5G_FCC4		FCC TCF	
					    			    					        	
"Vietnam禫玭"						VN						2G_WORLD					FCC/CE TCF	
					


*/

// Counter abbervation.
typedef enum _RT_COUNTRY_DEFINE_NUM
{
	RT_CTRY_AL,				//	"Albania焊ぺェㄈ"					
	RT_CTRY_DZ,             //  "Algeria焊のㄈ"					
	RT_CTRY_AG,             //  "Antigua & Barbuda矗ナ畄&ぺガ笷"	
	RT_CTRY_AR,             //  "Argentina"					
	RT_CTRY_AM,             //  "Armeniaㄈェㄈ"					
	RT_CTRY_AW,             //  "Aruba緗ぺ畄"						
	RT_CTRY_AU,             //  "Australia緿瑆"						
	RT_CTRY_AT,             //  "Austria而"						
	RT_CTRY_AZ,             //  "Azerbaijan峨臼"				
	RT_CTRY_BS,             //  "Bahamasぺ皑"					
	RT_CTRY_BB,             //  "Barbadosぺぺ吹"				
	RT_CTRY_BE,             //  "Belgiumゑ"					
	RT_CTRY_BM,             //  "Bermudaκ紏笷"					
	RT_CTRY_BR,             //  "Brazilぺ﹁"						
	RT_CTRY_BG,             //  "Bulgaria玂ㄈ"				
	RT_CTRY_CA,             //  "Canada"					
	RT_CTRY_KY,             //  "Cayman Islands秨耙竤畄"			
	RT_CTRY_CL,             //  "Chile醇"						
	RT_CTRY_CN,             //  "Chinaい瓣"						
	RT_CTRY_CO,             //  "Columbiaゑㄈ"				
	RT_CTRY_CR,             //  "Costa Rica吹笷兢"			
	RT_CTRY_CY,             //  "Cyprus峨隔吹"					
	RT_CTRY_CZ,             //  "Czech 倍"						
	RT_CTRY_DK,             //  "Denmarkう沉"					
	RT_CTRY_DO,             //  "Dominican Republicェ㎝瓣"	
	RT_CTRY_CE,             //  "Egypt甁の"	EG	2G_WORLD			
	RT_CTRY_SV,             //  "El Salvador履焊ニ"				
	RT_CTRY_EE,             //  "Estonia稲‵ェㄈ"					
	RT_CTRY_FI,             //  "Finland孽"						
	RT_CTRY_FR,             //  "France猭瓣"						
	RT_CTRY_DE,             //  "Germany紈瓣"					
	RT_CTRY_GR,             //  "Greece 镁"					
	RT_CTRY_GU,             //  "Guam闽畄"						
	RT_CTRY_GT,             //  "Guatemalaナ皑┰"				
	RT_CTRY_HT,             //  "Haiti"						
	RT_CTRY_HN,             //  "HondurasЩ常┰吹"				
	RT_CTRY_HU,             //  "Hungary"					
	RT_CTRY_IS,             //  "Iceland畄"					
	RT_CTRY_IN,             //  "India"						
	RT_CTRY_IE,             //  "Ireland稲焊孽"					
	RT_CTRY_IL,             //  "Israel︹"					
	RT_CTRY_IT,             //  "Italy竡"					
	RT_CTRY_JP,             //  "Japanらセ"						
	RT_CTRY_KR,             //  "Korea龙瓣"						
	RT_CTRY_LV,             //  "Latvia┰叉蝴ㄈ"					
	RT_CTRY_LT,             //  "Lithuaniaミ吵﹞"				
	RT_CTRY_LU,             //  "Luxembourg縞此躇"				
	RT_CTRY_MY,             //  "Malaysia皑ㄓ﹁ㄈ"				
	RT_CTRY_MT,             //  "Malta皑焊"					
	RT_CTRY_MX,             //  "Mexico茎﹁"					
	RT_CTRY_MA,             //  "Morocco集"					
	RT_CTRY_NL,             //  "Netherlands颤孽"				
	RT_CTRY_NZ,             //  "New Zealand﹁孽"				
	RT_CTRY_NO,             //  "Norway"						
	RT_CTRY_PA,             //  "Panamaぺ皑 "					
	RT_CTRY_PH,             //  "Philippines滇猾"				
	RT_CTRY_PL,             //  "Poland猧孽"						
	RT_CTRY_PT,             //  "Portugal覆靛"					
	RT_CTRY_RO,             //  "Romania霉皑ェㄈ"				
	RT_CTRY_RU,             //  "Russia玐霉吹"					
	RT_CTRY_SA,             //  "Saudi Arabia‵┰"			
	RT_CTRY_SG,             //  "Singapore穝℡"				
	RT_CTRY_SK,             //  "Slovakia吹ワ"				
	RT_CTRY_SI,             //  "Slovenia吹蝴ェㄈ"				
	RT_CTRY_ZA,             //  "South Africa玭獶"				
	RT_CTRY_ES,             //  "Spain﹁痁"					
	RT_CTRY_SE,             //  "Sweden风ㄥ"						
	RT_CTRY_CH,             //  "Switzerland风"				
	RT_CTRY_TW,             //  "Taiwan籓芖"						
	RT_CTRY_TH,             //  "Thailand瓣"					
	RT_CTRY_TR,             //  "Turkeyφㄤ"					
	RT_CTRY_UA,             //  "Ukraine疩孽"					
	RT_CTRY_GB,             //  "United Kingdom璣瓣"				
	RT_CTRY_US,             //  "United States瓣"				
	RT_CTRY_VE,             //  "Venezuela〆ず风┰"				
	RT_CTRY_VN,             //  "Vietnam禫玭"					
	RT_CTRY_MAX,            //  
	
}RT_COUNTRY_NAME, *PRT_COUNTRY_NAME;
    
// Scan type including active and passive scan.
typedef enum _RT_SCAN_TYPE_NEW
{
	SCAN_NULL,
	SCAN_ACT,
	SCAN_PAS,
	SCAN_BOTH,
}RT_SCAN_TYPE_NEW, *PRT_SCAN_TYPE_NEW;
    

// Power table sample.

typedef struct _RT_CHNL_PLAN_LIMIT
{
	u2Byte	Chnl_Start;
	u2Byte	Chnl_end;	
	
	u2Byte	Freq_Start;
	u2Byte	Freq_end;	
}RT_CHNL_PLAN_LIMIT, *PRT_CHNL_PLAN_LIMIT;

    
//    
// 2.4G Regulatory Domains
//
typedef enum _RT_REGULATION_DOMAIN_2G
{
	RT_2G_NULL,   
	RT_2G_WORLD,
	RT_2G_ETSI1,
	RT_2G_FCC1,
	RT_2G_MKK1,
	RT_2G_ETSI2	
	
}RT_REGULATION_2G, *PRT_REGULATION_2G;


//typedef struct _RT_CHANNEL_BEHAVIOR
//{
//	u1Byte	Chnl;
//	RT_SCAN_TYPE_NEW
//	
//}RT_CHANNEL_BEHAVIOR, *PRT_CHANNEL_BEHAVIOR;

//typedef struct _RT_CHANNEL_PLAN_TYPE
//{
//	RT_CHANNEL_BEHAVIOR		
//	u1Byte					Chnl_num;
//}RT_CHNL_PLAN_TYPE, *PRT_CHNL_PLAN_TYPE;

//
// 2.4G Channel Number
// Channel definition & number
//
#define CHNL_RT_2G_NULL \
        {0}, 0
#define CHNL_RT_2G_WORLD \
        {1,2,3,4,5,6,7,8,9,10,11,12,13}, 13
#define CHNL_RT_2G_WORLD_TEST \
        {1,2,3,4,5,6,7,8,9,10,11,12,13}, 13

#define CHNL_RT_2G_EFSI1 \
        {1,2,3,4,5,6,7,8,9,10,11,12,13}, 13
#define CHNL_RT_2G_FCC1 \
        {1,2,3,4,5,6,7,8,9,10,11}, 11
#define CHNL_RT_2G_MKK1 \
        {1,2,3,4,5,6,7,8,9,10,11,12,13,14}, 14
#define CHNL_RT_2G_ETSI2 \
        {10,11,12,13}, 4

//
// 2.4G Channel Active or passive scan.
//
#define CHNL_RT_2G_NULL_SCAN_TYPE \
        {SCAN_NULL}
#define CHNL_RT_2G_WORLD_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1,0,0}
#define CHNL_RT_2G_EFSI1_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CHNL_RT_2G_FCC1_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1}
#define CHNL_RT_2G_MKK1_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CHNL_RT_2G_ETSI2_SCAN_TYPE \
        {1,1,1,1}


//
// 2.4G Band & Frequency Section
// Freqency start & end / band number
//
#define FREQ_RT_2G_NULL \
        {0}, 0
        // Passive scan CH 12, 13
#define FREQ_RT_2G_WORLD \
        {2412, 2472}, 1
#define FREQ_RT_2G_EFSI1 \
        {2412, 2472}, 1
#define FREQ_RT_2G_FCC1 \
        {2412, 2462}, 1
#define FREQ_RT_2G_MKK1 \
        {2412, 2484}, 1
#define FREQ_RT_2G_ETSI2 \
        {2457, 2472}, 1


//    
// 5G Regulatory Domains
//
typedef enum _RT_REGULATION_DOMAIN_5G
{   
	RT_5G_NULL,
	RT_5G_WORLD,
	RT_5G_ETSI1,
	RT_5G_ETSI2,
	RT_5G_ETSI3,
	RT_5G_FCC1,	
	RT_5G_FCC2,
	RT_5G_FCC3,
	RT_5G_FCC4,
	RT_5G_FCC5,
	RT_5G_FCC6,
	RT_5G_FCC7,
	RT_5G_IC1,
	RT_5G_KCC1,
	RT_5G_MKK1,
	RT_5G_MKK2,
	RT_5G_MKK3,
	RT_5G_NCC1,
	
}RT_REGULATION_5G, *PRT_REGULATION_5G;

//
// 5G Channel Number
//
#define CHNL_RT_5G_NULL \
        {0}, 0
#define CHNL_RT_5G_WORLD \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140}, 19
#define CHNL_RT_5G_ETSI1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165}, 24
#define CHNL_RT_5G_ETSI2 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,149,153,157,161,165}, 22
#define CHNL_RT_5G_ETSI3 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165}, 24
#define CHNL_RT_5G_FCC1 \
        {36,40,44,48,149,153,157,161,165}, 9
#define CHNL_RT_5G_FCC2 \
        {36,40,44,48,52,56,60,64,149,153,157,161,165}, 13
#define CHNL_RT_5G_FCC3 \
        {36,40,44,48,52,56,60,64,149,153,157,161}, 12
#define CHNL_RT_5G_FCC4 \
        {149,153,157,161,165}, 5
#define CHNL_RT_5G_FCC5 \
        {36,40,44,48,52,56,60,64}, 8
#define CHNL_RT_5G_FCC6 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define CHNL_RT_5G_FCC7 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define CHNL_RT_5G_IC1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,149,153,157,161,165}, 20
#define CHNL_RT_5G_KCC1 \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140}, 19
#define CHNL_RT_5G_MKK1 \
        {36,40,44,48,52,56,60,64}, 8
#define CHNL_RT_5G_MKK2 \
        {100,104,108,112,116,120,124,128,132,136,140}, 11
#define CHNL_RT_5G_MKK3 \
        {56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 24
#define CHNL_RT_5G_NCC1 \
        {56,60,64,149,153,157,161,165}, 8  	

//
// 5G Channel Active or passive scan.
//
#define CHNL_RT_5G_NULL_SCAN_TYPE \
        {SCAN_NULL}
#define CHNL_RT_5G_WORLD_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CHNL_RT_5G_ETSI1_SCAN_TYPE \
        {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
#define CHNL_RT_5G_ETSI2_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,149,153,157,161,165}, 22
#define CHNL_RT_5G_ETSI3_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140,149,153,157,161,165}, 24
#define CHNL_RT_5G_FCC1_SCAN_TYPE \
        {36,40,44,48,149,153,157,161,165}, 9
#define CHNL_RT_5G_FCC2_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,149,153,157,161,165}, 13
#define CHNL_RT_5G_FCC3_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,149,153,157,161}, 12
#define CHNL_RT_5G_FCC4_SCAN_TYPE \
        {149,153,157,161,165}, 5
#define CHNL_RT_5G_FCC5_SCAN_TYPE \
        {36,40,44,48,52,56,60,64}, 8
#define CHNL_RT_5G_FCC6_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define CHNL_RT_5G_FCC7_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 20
#define CHNL_RT_5G_IC1_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,149,153,157,161,165}, 20
#define CHNL_RT_5G_KCC1_SCAN_TYPE \
        {36,40,44,48,52,56,60,64,100,104,108,112,116,120,124,128,132,136,140}, 19
#define CHNL_RT_5G_MKK1_SCAN_TYPE \
        {36,40,44,48,52,56,60,64}, 8
#define CHNL_RT_5G_MKK2_SCAN_TYPE \
        {100,104,108,112,116,120,124,128,132,136,140}, 11
#define CHNL_RT_5G_MKK3_SCAN_TYPE \
        {56,60,64,100,104,108,112,116,136,140,149,153,157,161,165}, 24
#define CHNL_RT_5G_NCC1_SCAN_TYPE \
        {56,60,64,149,153,157,161,165}, 8  	

//    
// Global Regulation
//
typedef enum _RT_REGULATION_COMMON
{
	RT_WORLD,   
	RT_FCC,   
	RT_MKK,
	RT_ETSI,
	RT_IC,
	RT_CE,
	RT_NCC,
	
}RT_REGULATION_CMN, *PRT_REGULATION_CMN;



//    
// Special requirement for different regulation domain.
// For internal test or customerize special request.
//
typedef enum _RT_CHNLPLAN_SREQ
{
	RT_SREQ_NA						= 0x0,
	RT_SREQ_2G_ADHOC_11N			= 0x00000001,
	RT_SREQ_2G_ADHOC_11B			= 0x00000002,
	RT_SREQ_2G_ALL_PASS				= 0x00000004,
	RT_SREQ_2G_ALL_ACT				= 0x00000008,	
	RT_SREQ_5G_ADHOC_11N			= 0x00000010,
	RT_SREQ_5G_ADHOC_11AC			= 0x00000020,
	RT_SREQ_5G_ALL_PASS				= 0x00000040,
	RT_SREQ_5G_ALL_ACT				= 0x00000080,
	RT_SREQ_C1_PLAN					= 0x00000100,	
	RT_SREQ_C2_PLAN					= 0x00000200,	
	RT_SREQ_C3_PLAN					= 0x00000400,	
	RT_SREQ_C4_PLAN					= 0x00000800,	
	RT_SREQ_NFC_ON					= 0x00001000,	
	RT_SREQ_MASK					= 0x0000FFFF,   /* Requirements bit mask */
	
}RT_CHNLPLAN_SREQ, *PRT_CHNLPLAN_SREQ;


//
// RT_COUNTRY_NAME & RT_REGULATION_2G & RT_REGULATION_5G transfer table
// 
//
typedef struct _RT_CHANNEL_PLAN_COUNTRY_TRANSFER_TABLE
{   
	//
	// Define countery domain and corresponding 
	//	
	RT_COUNTRY_NAME		Country_Enum;
	char				Country_Name[3];
	
	//char		Domain_Name[12];
	RT_REGULATION_2G	Domain_2G;	

	RT_REGULATION_5G	Domain_5G;	

	RT_CHANNEL_DOMAIN	RtChDomain;
	//u1Byte		Country_Area;
    
}RT_CHNL_CTRY_TBL, *PRT_CHNL_CTRY_TBL;
    

#define		RT_MAX_CHNL_NUM_2G		13
#define		RT_MAX_CHNL_NUM_5G		44	

// Power table sample.

typedef struct _RT_CHNL_PLAN_PWR_LIMIT
{
	u2Byte	Chnl_Start;
	u2Byte	Chnl_end;
	u1Byte	dB_Max;
	u2Byte	mW_Max;
}RT_CHNL_PWR_LIMIT, *PRT_CHNL_PWR_LIMIT;


#define		RT_MAX_BAND_NUM			5

typedef struct _RT_CHANNEL_PLAN_MAXPWR
{
//	STRING_T
	RT_CHNL_PWR_LIMIT	Chnl[RT_MAX_BAND_NUM];
	u1Byte				Band_Useful_Num;

	
}RT_CHANNEL_PLAN_MAXPWR, *PRT_CHANNEL_PLAN_MAXPWR;


//
// Power By Rate Table.
//



typedef struct _RT_CHANNEL_PLAN_NEW
{   
	//
	// Define countery domain and corresponding 
	//
	//char		Country_Name[36];
	//u1Byte		Country_Enum;
	
	//char		Domain_Name[12];

	
	PRT_CHNL_CTRY_TBL		pCtryTransfer;
	
	RT_CHANNEL_DOMAIN		RtChDomain;	

	RT_REGULATION_2G		Domain_2G;

	RT_REGULATION_5G		Domain_5G;	

	RT_REGULATION_CMN		Regulator;

	RT_CHNLPLAN_SREQ		ChnlSreq;
	
	//RT_CHNL_PLAN_LIMIT		RtChnl;
		
	u1Byte	Chnl2G[MAX_CHANNEL_NUM];				// CHNL_RT_2G_WORLD
	u1Byte	Len2G;
	u1Byte	Chnl2GScanTp[MAX_CHANNEL_NUM];			// CHNL_RT_2G_WORLD_SCAN_TYPE
	//u1Byte	Freq2G[2];								// FREQ_RT_2G_WORLD

	u1Byte	Chnl5G[MAX_CHANNEL_NUM];				
	u1Byte	Len5G;
	u1Byte	Chnl5GScanTp[MAX_CHANNEL_NUM];
	//u1Byte	Freq2G[2];								// FREQ_RT_2G_WORLD

	RT_CHANNEL_PLAN_MAXPWR	ChnlMaxPwr;
	
    
}RT_CHANNEL_PLAN_NEW, *PRT_CHANNEL_PLAN_NEW;
    
    
#endif	// __RT_CHANNELPLAN_H__
    
    
    
    
 
