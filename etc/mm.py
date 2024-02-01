import sys
import re

def mX_type ( width ) :
    return [ '_', 's', 'd', 't', 'q', 'q' ][width]
def mX_Type ( width ) :
    return [ '_', 'S', 'D', 'T', 'Q', 'Q' ][width]

def all_replace( line, Tc, m_pattern, M_pattern ) :
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
        s_pattern = ''
        for i in range( 1, Tc ) :
                s_pattern = s_pattern + fmt.replace( '@{i}@', str(i) )
        fmt = '@@{' + fmt + '}@@'
        line = line.replace( fmt, s_pattern )
    return line


def Read_Data( infile, Tc ) :

    m_pattern = mX_type( Tc )
    M_pattern = mX_Type( Tc )

    with open( infile, mode = 'r' ) as file :
        while True :
            line = file.readline()
            if not line :
                break
            line = line.rstrip('\n')

            if '@@@{' in line :
                line = line.replace( '@@@{','' )
                a_list = line.split()
                pattern = '@{{{M}}}@'.format( M=a_list[0] )
                data = {}
                i = 0
                while True :
                    line = file.readline()
                    line = line.rstrip('\n')
                    if '}@@@' in line :
                        break
                    data[i] = line
                    i = i + 1
                for itr in range(1, len(a_list) ) :
                    c_pattern = a_list[itr]
                    for i in range( len(data) ) :
                        line = data[i]
                        if pattern in line :
                            line = line.replace( pattern, c_pattern )
                        line = all_replace( line, Tc, m_pattern, M_pattern )
                        print( line )
                continue

            line = all_replace( line, Tc, m_pattern, M_pattern )

            if '@@include' in line :
                a_list = line.split()
                infile2 = a_list[1].replace('"','')
                Read_Data( infile2, Tc )
                continue

            print( line )


if __name__ == '__main__' :

    args = sys.argv
    argc = len( args )

    infile  = args[1]
    Tc  = int(args[2])

    Read_Data( infile, Tc )

