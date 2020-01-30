set torture_eval_before_compile {

    set compiler_conditional_xfail_data {
        "Bogus label refernce in debugging output"
	"arm-*-* thumb-*-*" \
	{"-O2 -g" ""} \
	{"" }
	}    
}

return 0
