set torture_eval_before_execute {

    set compiler_conditional_xfail_data {
        "I sure wish I knew why this was hosed" \
	"arm-*-* thumb-*-* fr30-*-elf" \
	{"-O2 -finline-functions"} \
	{"" }
	}    
}

return 0

