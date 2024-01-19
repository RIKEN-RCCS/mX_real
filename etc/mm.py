import sys

def Read_Data( infile ) :

    with open( infile, mode = 'r' ) as file :

        while True :
            line = file.readline()
            if not line :
                break

            if '@@include' in line :

                a_list = line.split()
                infile2 = a_list[1].replace('"','')
                Read_Data( infile2 )

            else :

                print( line,end='' )


if __name__ == '__main__' :

    args = sys.argv
    argc = len( args )

    infile  = args[1]
    Read_Data( infile )

