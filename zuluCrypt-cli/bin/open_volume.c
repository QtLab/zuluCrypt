/*
 * 
 *  Copyright (c) 2011
 *  name : mhogo mchungu 
 *  email: mhogomchungu@gmail.com
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 2 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 * 
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "includes.h"
#include <errno.h>

static int status_msg( int st )
{
	switch ( st ){
		case 0 : printf( "SUCCESS: Volume opened successfully\n" ) ;								break ;
		case 1 : printf( "ERROR: failed to mount ntfs file system using ntfs-3g,is ntfs-3g package installed?\n" ) ;		break ;					
		case 2 : printf( "ERROR: there seem to be an open volume accociated with given address\n" );				break ;				
		case 3 : printf( "ERROR: no file or device exist on given path\n" ) ; 							break ;		
		case 4 : printf( "ERROR: wrong passphrase\n" );										break ;			
		case 5 : printf( "ERROR: could not create mount point, invalid path or path already taken\n" ) ;			break ;		
		case 6 : printf( "ERROR: passphrase file does not exist\n" );								break ;	
		case 8 : printf( "ERROR: failed to open volume\n" );									break ;	
		case 9 : printf( "ERROR: mount point path is already taken\n" );							break ;					 
		case 10: printf( "ERROR: \",\" ( comma ) is not a valid mount point\n" );						break ;
		case 11: printf( "ERROR: one or more required argument(s) for this operation is missing\n" );				break ;				
		case 12: printf( "ERROR: could not get a lock on /etc/mtab~\n" );							break ;
		case 13: printf( "ERROR: wrong argument for mode, valid options are \"rw\" or \"ro\"\n" );				break ;
		case 14: printf( "ERROR: could not get enought memory to hold the key file\n" );					break ;
		case 15: printf( "ERROR: failed to open volume and failed to close the mapper, advice to do it manunally\n" );		break ;
		case 16: printf( "ERROR: could not resolve full path of mount point\n" );						break ;
		case 17: printf( "ERROR: could not resolve full path of device address\n" );						break ;
		case 18: printf( "ERROR: -O and -m options can not be used together\n" );						break ;
		case 19: printf( "ERROR: insufficient privilege to search mount point path\n" );					break ;	
		case 20: printf( "ERROR: insufficient privilege to open device\n" );							break ;	
		case 21: printf( "ERROR: insufficient privilege to create a mount point\n" );						break ;	
		case 22: printf( "ERROR: insufficient privilege to open key file for reading\n" );					break ;					
		default: printf( "ERROR: unrecognized error with status number %d encountered\n",st );
	}
	return st ;
}

/*
 * open_volume function below can be devided into two, the first part is before the mount point folder is created and the 
 * other part is after. This function is called after the mount point is created to see if it the mount point folder
 * should be removed first before calling the above function.The above function is called directly when "open_volume"
 * function is to be exited before the mount point is created. * 
 */
static int status_msg_1( int st,const struct_opts * opts )
{
	if( opts->open_no_mount == -1 && st != 0 )
		rmdir( opts->mount_point ) ;
	return status_msg( st ) ;
}

int open_volumes( const struct_opts * opts,const char * mapping_name,uid_t uid )
{
	int nmp                  = opts->open_no_mount ;
	int i                    = opts->interactive_passphrase ;
	const char * dev         = opts->device ;
	const char * mount_point = opts->mount_point ;
	const char * mode        = opts->mode ;
	const char * source      = opts->key_source ;
	const char * pass        = opts->key ;
	
	string_t passphrase  ;	
	string_t m_name  ;	
	string_t data ;
	
	const char * cpass ;	
	const char * cname ;
	
	char * device ;
	char * cpoint = NULL ;
	
	size_t len ;
	int st = 0 ;
	
	/*
	 * This function is defined at "is_path_valid.c"
	 * It makes sure the path exists and the user has atleast reading access to the path.
	 * 
	 * The importance of the function is explained where it is defined.
	 */
	switch( can_open_path_for_reading( dev,uid ) ){
		case 0 : break ;
		case 1 : return status_msg( 20 ) ;
		default: return status_msg( 3 ) ;
	}
	
	device = realpath( dev,NULL ) ;
	if( device == NULL )
		return status_msg( 17 ) ;
	
	if( mode == NULL ) 
		return status_msg( 11 ) ;
	
	if( strncmp( mode,"ro",2 ) != 0 )
		if ( strncmp( mode,"rw",2 ) != 0 )
			return status_msg( 13 ) ;
	
	if( nmp == 1 && mount_point != NULL )
		return status_msg( 18 ) ;
	
	if( nmp == -1 ){
		if( mount_point == NULL )
			return status_msg( 11 ) ;
		
		if( strlen( mount_point ) == 1 )
			if ( strcmp( mount_point,"," ) == 0 )
				return status_msg( 10 ) ;
		/*
		 * defined in security.c
		 */
		switch( create_mount_point( mount_point,uid ) ){
			case 2 : return status_msg( 5 ) ;
			case 1 : return status_msg( 21 ) ;
		}
		
		cpoint = realpath( mount_point,NULL ) ;
		if( cpoint == NULL )
			return status_msg_1( 16,opts ) ;
	}		

	/*
	 * This function is defined at "create_mapper_name.c"
	 * 
	 * Explanation for what it does is explained where it is defined.	  * 
	 */
	m_name = create_mapper_name( device,mapping_name,uid,OPEN ) ;
	
	cname = StringContent( m_name ) ;
	
	if ( i == 1 ){
		printf( "Enter passphrase: " ) ;		
		passphrase = get_passphrase();	
		printf( "\n" ) ;
		cpass = StringContent( passphrase ) ;
		len = StringLength( passphrase ) ;
		st = open_volume( device,cname,cpoint,uid,mode,cpass,len ) ;
		StringDelete( &passphrase ) ;
	}else{
		if( source == NULL || pass == NULL )
			return status_msg_1( 11,opts ) ;
		
		if( strcmp( source,"-p" ) == 0 ){
			cpass = pass ;
			len = strlen(pass) ;
			st = open_volume( device,cname,cpoint,uid,mode,cpass,len ) ;		
		}else if( strcmp( source,"-f" ) == 0 ){		
			/*
			 * function is defined at "security.c"
			 */
			switch( get_pass_from_file( pass,uid,&data ) ){
				case 1 : return status_msg_1( 6,opts ) ; 
				case 2 : return status_msg_1( 14,opts ) ; 				
				case 4 : return status_msg_1( 22,opts ) ;
			}
			cpass = StringContent( data ) ;
			len = StringLength( data ) ;
			st = open_volume( device,cname,cpoint,uid,mode,cpass,len ) ;
			StringDelete( &data ) ;
		}
	}
		
	StringDelete( &m_name ) ;
	
	free( cpoint ) ;
	free( device ) ;
	
	st = status_msg_1( st,opts ) ;
	
	check_invalid_key( opts->device ) ;
	
	return st ;	
}
