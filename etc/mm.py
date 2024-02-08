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

    data = {}
    i = 0
    with open( infile, mode = 'r' ) as file :
        while True :
            line = file.readline()
            if not line :
                break
            data[i] = line.rstrip('\n')
            i = i + 1

    i = 0
    while i < len(data) :

        line = data[i]

        if '@@@{' in line :
            line = line.replace( '@@@{','' )
            a_list = line.split()
            pattern = '@{{{M}}}@'.format( M=a_list[0] )

            i = i + 1
            local_data = {}
            j = 0
            while True :
                line = data[i+j]
                if '}@@@' in line :
                    break
                local_data[j] = line
                j = j + 1

            for itr in range(1, len(a_list) ) :
                c_pattern = a_list[itr]
                j = 0
                while j < len(local_data) :
                    line = local_data[j]
                    if pattern in line :
                        line = line.replace( pattern, c_pattern )
                    line = all_replace( line, Tc, m_pattern, M_pattern )

                    print( line )
                    j = j + 1

            i = i + 1 + len(local_data)
            continue

        line = all_replace( line, Tc, m_pattern, M_pattern )

        if '@@include' in line :
            a_list = line.split()
            infile2 = a_list[1].replace('"','')
            Read_Data( infile2, Tc )
            i = i + 1
            continue

        print( line )
        i = i + 1


if __name__ == '__main__' :

    args = sys.argv
    argc = len( args )

    infile  = args[1]
    Tc  = int(args[2])

    Read_Data( infile, Tc )

