import sys
import re

def mX_type ( width ) :
    return [ '_', 's', 'd', 't', 'q', 'q' ][width]
def mX_Type ( width ) :
    return [ '_', 'S', 'D', 'T', 'Q', 'Q' ][width]

def Read_Data( infile, Tc ) :

    m_pattern = mX_type( Tc )
    M_pattern = mX_Type( Tc )

    with open( infile, mode = 'r' ) as file :

        while True :
            line = file.readline()
            if not line :
                break
            line = line.rstrip('\n')

            if '@{L}@' in line :
                line = line.replace( '@{L}@', str(Tc) )

            if '@{m}@' in line :
                line = line.replace( '@{m}@', m_pattern )
            if '@{M}@' in line :
                line = line.replace( '@{M}@', M_pattern )

            if '@@{' in line :
                fmt = line
                fmt = re.sub( '.*@@{', '', fmt )
                fmt = re.sub( '}@@.*', '', fmt )
                s = ''
                for i in range( 1, Tc ) :
                        s = s + fmt.replace( '@{i}@', str(i) )
                fmt = '@@{' + fmt + '}@@'
                line = line.replace( fmt, s )

            if '@@include' in line :

                a_list = line.split()
                infile2 = a_list[1].replace('"','')
                Read_Data( infile2, Tc )

            else :

                print( line )


if __name__ == '__main__' :

    args = sys.argv
    argc = len( args )

    infile  = args[1]
    Tc  = int(args[2])

    Read_Data( infile, Tc )

