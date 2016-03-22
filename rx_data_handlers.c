#include "xc.h"
#include "stdio.h"

void handle_remote_control( );
void handle_remote_control_DATA1( );
//
// Remote Routine
//
// interprets and testes receive codes
// Does Normal Operation and Learn Mode
//
// INPUT:  Buffer contains the received code word
//         
// OUTPUT: S0..S3 and LearnOut 
//

void handle_data( )
{
  unsigned char free_mem,
      recived_CRC;

  // a frame was received and is stored in the receive buffer
  // move it to decryption Buffer, and restart receiving

  //send buffer for usart for debug
  printf( "\r\n" );
  for ( i = 0; i < RX_BUFF_LEN; i++ )
    {
      printf( "%X ", Buffer[i] );
    }

  recived_CRC = CRC; //save crc
  CRC = 0xff; //set dumy crc byte
  crc = BlockCRC( 0, &Buffer[0], RX_BUFF_LEN ); //calculate crc

  check_sum = 0; //calc checksum for remote cont
  for ( i = 0; i < RX_BUFF_LEN - 1; i++ )
    {
      check_sum += Buffer[i];
    }


  GREEN_LED = 1;

  GREEN_LED = 0;
  if ( crc == recived_CRC ) //if massage is is ok
    {
      RED_LED = 1;
      printf( "\r\n Good CRC" );
      if ( recive_testing ) //if recive testing enabled sound buzz on every recive
        BuzzTimer = LEARN_BUZZ_TIME; //sound buzz on every recive
      RED_LED = 0;
    }
  else if ( check_sum == recived_CRC )
    {
      RED_LED = 1;
      printf( "\r\n Good CS" );
      if ( recive_testing ) //if recive testing enabled sound buzz on every recive
        BuzzTimer = LEARN_BUZZ_TIME;
      RED_LED = 0;
    }
  else
    {
      RED_LED = 1;
      printf( "\r\n Bad CRC and CS" );
      RED_LED = 0;
      if ( recive_testing ) //if recive testing enabled sound buzz on every recive
        BuzzTimer = LEARN_BUZZ_TIME * 4;
      return; //exit without doing nothing
    }

  Saved_data = Data; //save the data

  transmiter_type = Data & Type; //extract the transmiter type
  //printf("\r\n data is %x  type is %x",Data,transmiter_type);
  transmiter_type_2 = Data & Type1;  //extract the transmiter type


   //test if it is a master remote controle
  master_remote_founed = 0;
  if ( transmiter_type == REMOTE_CONT )
    {
      if ( IDLo == eeprom_read( MASTER_REMOTE_CONTROLE_IDLo_EE_ADD ) )
        {
          if ( IDHi == eeprom_read( MASTER_REMOTE_CONTROLE_IDHi_EE_ADD ) )
            master_remote_founed = 1;
        }
    }
  else  //if its a display or transmiter
    {
      //if its a new transmiter on old recever or old transmiter on new reciver ignore it
      if( transmiter_type_2 )     //if its a new Crane Display module
        {
          if ( unit_type == CRANE_CAMERA_MODULE_OLD_CARD ) //and the recever is also a new unit then handle the video
            {
              printf( "\r\n  unit is new camera module and transmiter is old fab transmiter so ignoring trasmiter" );
              return;           //so ignor it
            }
        }
      else      //its an old fab transmiter
        {
          if ( unit_type != CRANE_CAMERA_MODULE_OLD_CARD ) //and its an old unit then handle the command
            {
              printf( "\r\n  unit is working like an old camera module and transmiter is new screen so ignoring trasmiter" );
              return;           //so ignor it
            }
        }
    }
  

  if ( FLearn ) // Learn Mode
    {
      printf( "\r\n We Are in Learn mode" );
      a = Find( ); //it is already exist so exit
      if ( a < MAX_USER - 1 )
        {
          printf( "\r\n Known Unit Recived ,ignoring unit\r\n" );
          return;
        }

      if ( master_remote_founed )
        {
          printf( "\r\n master_remote_founed ,ignoring unit \r\n" );
          return;
        }





      printf( "\r\n New Unit Recived" );

      free_mem = check_empty_space( transmiter_type ); // look for new memory space fir the unit

      if ( free_mem > MAX_USER - 1 )
        {
          printf( "\r\n no more  place in memory for new unit ,ignoring unit" );
          return; //sory no moer place so exit
        }

      //this block is testing for 4 continuse valid reciving of the
      // same code before saving it into the eeprom
      //  !! ATT memcmp() func returns 0 if the 2 buffers are equal
      if ( !memcmp( &IDLo, LearnBuffer, EL_SIZE ) ) //did we recived this code before
        Learn_Recive_counter++; //yes increment counter
      else
        Learn_Recive_counter = 0; //no reset counter to start all over again

      memcpy( LearnBuffer, &IDLo, EL_SIZE ); //save the new recived word for next time

      //LearnBuffer[], &Buffer[IDLo]
      //LearnBuffer, &Buffer[IDLo]

      printf( "\r\n Learn_Recive_counter=%2d ", Learn_Recive_counter );
      if ( Learn_Recive_counter < 2 ) //is it the 3rd time of valid recive
        return; //no so exit and wait for the next recive

      /* ok !! start learningnew transmiter */
      Learn_Recive_counter = 0; //reset counter to start all over again for next transmiter


      /* now free_mem is pointing to a valid memory location */
      IDWrite( free_mem ); //so write the new transmiter in memory
      // fail if no memory available
      if ( transmiter_type == TARGET ) //if its a transmiter
        {
          my_eeprom_write( EEPROM_TRANSMITER_ADD_DATA + free_mem * 2, SyncHi ); // write target_counter msb to eeprom
          my_eeprom_write( EEPROM_TRANSMITER_ADD_DATA + free_mem * 2 + 1, SyncLo ); // write target_counter lsb to eeprom
          tmpI = SyncHi; //get the hit target_counter from the target
          tmpI <<= 8;
          tmpI += SyncLo;
          target_counter_at_lurn[free_mem] = tmpI; //for work later
          target_hit_conter[free_mem] = 0; //reset the curent counter
        }
      else //if its a remote controle
        {
          NOP( );
        }

      //BuzzTimer=RECIVE_BUZZ_TIME;								//sound the buzz for sucsesfull lern
      //		for(i=0;i<3;i++)												//in sucsesfull lern
      //			{																//bink the learnd display massage 3 times
      //			buzz_on;
      //			switch(free_mem)											//and display on the screan the learned unit
      //				{
      //				case 0:display_seg_massage (T1);	break;
      //				case 1:display_seg_massage (T2);	break;
      //				case 2:display_seg_massage (T3);	break;
      //				case 3:display_seg_massage (T4);	break;
      //				case 4:display_seg_massage (T5);	break;
      //				case 5:display_seg_massage (R1);	break;
      //				case 6:display_seg_massage (R2);	break;
      //				case 7:display_seg_massage (R3);	break;
      //				}
      //			DelayMs(250);	DelayMs(250);
      //			buzz_off;
      //			all_seg_off();
      //all_leds_off();
      //			DelayMs(250);	DelayMs(250);
      //			}
      display_Title( 0, 0, WHITE, NO_BLINK, "TRANSMITER LEARNED" );
      curent_title = TRANSMITER_LEARNED_TITLE;
      printf( "\r\n New Unit Learned in Add %2d", free_mem );
      //      buzz_on;
      blink_GREEN_LED_and_RED_LED( 5, 100 );
      //      buzz_off;

      CTLearn = 0; // exit learn after every successfull Learn
      FLearn = 0;
      title_timeout = _1Sec_IN_100Msec_UNITS;           //display the batt in 1 sec from now
      //send_Title_Command(TITLE_CLEAR);

      if( !master_remote_founed )
        Learn_Window = 0;                     //exit learn window if its a TARGET                    //exit learn window if its a TARGET

    }
  else // Normal Mode of operation
    {
      a = Find( );
      if ( a >= MAX_USER && !master_remote_founed ) //if we didnt founed a learned unit of a master remote controle-1
        return; //return

      if ( transmiter_type == TARGET ) //if its 1 its a transmiter
        {
          //transmiter_type_2 = Data & Type1;  //extract the transmiter type
          if( transmiter_type_2 )
            {
              printf( "\r\n knowen CRANE_VISIN Screen" );
              NOP( );
            }
          else
            {
              printf( "\r\n knowen Target" );
              NOP( );
            }
          //	printf("\r\n data is %x  type is %x",Data,transmiter_type);
          //          tmpI = SyncHi;																			//get the hit counter from the target
          //          tmpI <<= 8;
          //          tmpI += SyncLo;
          //
          //          if( target_hit_conter[a] != tmpI - target_counter_at_lurn[a] )		//is it the firest reciption of this shot
          if ( Sync != Sync_Last )//if its a new transmition
            {
              printf( " ,1st time" );
              if( transmiter_type_2 )     //if its a new Crane Display module
                {
                  NOP( );              //if its a CRANE VISION Screen continue normal handline
                }
              else      //its an old fab transmiter
                {
                  //remote_sw_stat |= 0x08;
                  Data = ON_OFF_KEY;          //if learned transmiter recived memic sw press => like MOVE_TO_PRESET_SW
                  NOP( );                       //since every SW press is valied
                }

              //BuzzTimer=NORMAL_BUZZ_TIME;					//yes so souned buzz only on the firest reciotion
              //display_on_count_doun_timer=DISP_ON_TIME;			//restart the display timer
              //curent_disp=a;						//display firest the last receved transmiter
              //remote_sw_stat |= 0x08;                                       //if transmiter recived memic sw press => like MOVE_TO_PRESET_SW;
              handle_remote_control_DATA1( ); //handle the VIDEO CH data
              handle_remote_control( ); //and the rest of the data
              transmiter_detectd = 1;
            }
          else
            printf( " ,repeat" );

          Sync_Last = Sync;
          //target_hit_conter[a] = tmpI - target_counter_at_lurn[a];		//and calculate the curent hit counter
          //transmiter_detectd=1;
        }
      else //if its 0 its a remote controle
        {
          if ( master_remote_founed )
            printf( "\r\n Master Remote Control" );
          else
            printf( "\r\n knowen Remote" );
          NOP( );
          handle_remote_control( );
          transmiter_detectd = 1;
        }
    } // normal mode
} // remote

/*
 * handle the DATA1 word from the remote controle
 */
void handle_remote_control_DATA1( )
{
  unsigned char i;

  if ( unit_type != CRANE_CAMERA_MODULE_OLD_CARD ) //if its a new unit then handle the video
    {
      /* handle video camera*/
      i = Data1;
      i &= 0x03; //get thew channel data saved in the 2 LSB Bits
      printf( " \n\r Data1=%2x ", i );
      video_ch = eeprom_read( VIDEO_CH_EE_ADD ); //read the last video channel from the eeprom
      if ( i != video_ch ) //if we need to set bew channel
        {
          video_ch = i;
          eeprom_write( VIDEO_CH_EE_ADD, video_ch ); //save the new channal
          set_video_ch( video_ch ); //and set it in board
        }
    }
}

void handle_remote_control( )
{
  unsigned char i;

  i = Data;
  i &= 0x0f;
  printf( " \n\r i=%2x ", i );
  remote_sw_stat = 0;
  switch ( i ) //and activate the lernd remote controle functiones
    {
      case ON_OFF_KEY:
        remote_sw_stat = MOVE_TO_PRESET;
        NOP( );
        printf( " \n\r ON_OFF_KEY presed" );
        break;

      case DISP_UP_KEY:
        remote_sw_stat = ZOOM_IN;
        NOP( );
        printf( " \n\r DISP_UP_KEY" );
        break;

      case DISP_DN_KEY:
        remote_sw_stat = ZOOM_OUT;
        NOP( );
        printf( " \n\r DISP_DN_KEY" );
        break;

      case RST_KEY:
        remote_sw_stat = SAVE_CURENT_LOCATION;
        NOP( );
        printf( " \n\r RST_KEY" );
        break;

        //   		case RESET_ALL_KEY:
        //   					NOP();
        //   					printf(" \n\r RESET_ALL_KEY");
        //			        	break;

        //   		case LRN_KEY:
        //   					NOP();
        //   					printf(" \n\r LRN_KEY");
        //			        	lrn_sw_presed_on_remote=1;
        //						break;

      case REM_ERASE_ALL_MEMORY:
        remote_sw_stat = ZOOM_IN + SAVE_CURENT_LOCATION;
        NOP( );
        printf( " \n\r REM_ERASE_ALL_MEMORY" );
        break;

      case REM_ERAZE_ALL_TRANSMITERS:
        remote_sw_stat = ZOOM_IN + ZOOM_OUT + SAVE_CURENT_LOCATION;
        NOP( );
        printf( " \n\r REM_ERAZE_ALL_TRANSMITERS" );
        break;

      case REM_LEARN_TRANSMITER:
        remote_sw_stat = MOVE_TO_PRESET + SAVE_CURENT_LOCATION;
        lrn_sw_presed_on_remote = 1;
        NOP( );
        printf( " \n\r REM_LEARN_TRANSMITER" );
        break;
    }
}


