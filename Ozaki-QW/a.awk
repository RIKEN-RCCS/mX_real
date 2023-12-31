BEGIN{
	print "#ifndef QXW_H"
	print "#define QXW_H"
	print ""

	# 4lines to be embedded here as is for the copyright notice
	getline; print;
	getline; print;
	getline; print;
	getline; print;

	print ""
	print "// C++-fied version 2023.11.2 (converted from Ozaki's code)"
	print "// by Toshiyuki Imamura (RIKEN, R-CCS)"
	print ""
}
/^\/\//{
	gsub(/^\/\/[A-Za-z\-]/,"@&");
	gsub(/@\/\//,"// ");
       	print;
       	next;
}
{
	if ( $0~/^void sqr_PA_QTW.*;/ )  {
	print "namespace QxW {\n\n"
	print "template < typename T > struct fp_const {};"
	print "template <> struct fp_const<float> {"
	print "  static inline auto constexpr zero() { return 0.0f; }"
	print "  static inline auto constexpr one()  { return 1.0f; }"
	print "  static inline auto constexpr half() { return 0.5f; }"
	print "};"
	print "template <> struct fp_const<double> {"
	print "  static inline auto constexpr zero() { return 0.0; }"
	print "  static inline auto constexpr one()  { return 1.0; }"
	print "  static inline auto constexpr half() { return 0.5; }"
	print "};\n"
	next;
	}

       	gsub(/float[ ]*\*[ ]*,/,"float \\&,");
       	gsub(/float[ ]*\*[ ]*)/,"float \\&)");

       	gsub(/float[ ]*\*/,"float \\&");
       	#gsub(/float[ ]+[a-zA-Z]/,"@&");
       	#gsub(/@float /,"float const\\& ");

       	gsub(/float[ ]*,/,"float const\\&,");

	gsub(/^[ \t]*float const&/, "&@");
	gsub(/float const&@/,"float");

	gsub(/[\(+-/ \t]\*[a-z]+[0-9]*/,"@[&]@");
        gsub(/@\[[\(-/]/,"@&]$$@[");
        gsub(/\]\$\$/,"");
        gsub(/@@\[/,"");
	gsub(/@\[ /,"@[");
	gsub(/@\[\*/," ");
	gsub(/\]@/,"");


	gsub(/&[erstp][0-9]/,"[@]&");
	gsub(/&[dse][hel]/,"[@]&");
	gsub(/\[@]&/,"");
	gsub(/,&t);/,",t);");

	gsub(/fma/,"std::fma");
	gsub(/sqrt\(/,"std::sqrt(");
	gsub(/,float/,", float");

	gsub(/=[ ]*0;/,"= fp_const<T>::zero();");
	gsub(/0\.5/,"fp_const<T>::half()");
	gsub(/1\//,"fp_const<T>::one()/");

	if ( $0~/^void/ ) {
#	gsub(/^void /,"template < typename T > inline &");
	gsub(/^void /,"template < typename T > __always_inline &");
	}
	gsub(/float/,"T");

	gsub(/\( /,"(");
	gsub(/ \)/,")");
	gsub(/\+ /,"+");
	gsub(/ \+/,"+");
	gsub(/- /,"-");
	gsub(/ -/,"-");
	gsub(/\* /,"*");
	gsub(/ \*/,"*");
	gsub(/\/ /,"/");
	gsub(/ \//,"/");

	gsub(/[0-9a-z\)][\+\-\*\/,][0-9a-z\(]/,"@[&]@");
	gsub(/@[\+\-\*\/,][0-9a-z\(]/,"@[&]@");
	gsub(/[0-9a-z\)][\+\-\*\/,]@/,"@[&]@");
	gsub(/@\[[0-9a-z\)@]/,"& ");
	gsub(/[0-9a-z\(@]\]@/," &");
	gsub(/@\[/,"");
	gsub(/\]@/,"");

	gsub(/=[ ]*/,"=");
	gsub(/[a-z0-9]=/,"&@");
	gsub(/=@/," =");
	gsub(/=[a-z0-9\-\+\(]/,"@&");
	gsub(/@=/,"= ");

	gsub(/,\-/,", -");
	gsub(/ ,/,",");
	gsub(/\( /,"(");
	gsub(/ \)/,")");
	gsub(/\+\(/,"+ (");
	gsub(/\-\(/,"- (");
	gsub(/\)\+/,") +");
	gsub(/\)\-/,") -");

}
/^template/{

	if( $0~/ TwoSum/ ) {
		gsub( /T const&/, "T const" );
	}
	if( $0~/ FastTwoSum/ ) {
		gsub( /T const&/, "T const" );
	}
	if( $0~/ TwoProductFMA/ ) {
		gsub( /T const&/, "T const" );
	}
#	gsub( /T[ ]*&/, "T const \\&" );

	gsub(/T [a-zA-Z0-9]+[,\)]/,"@&");
	gsub(/@T/,"T const");
}
{ print }
END {
       	print "\n}"
	print "\n#endif"
}

