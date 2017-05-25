const uint32_t fpscan_tagdata[] = {

// 151A iota
//0x00000001, 0x07000005, 0x00580010, 0x00000000,
//0x00500058, 0x00000000, 0x00000050, 
0x05000002, 0x00011804, 
0x00000020, //was 220

0x05000003, 0x0002400c, 
0x00000002, 
0x000000e0, //was 1e0

0x05000002, 0x00014018,
0x00000046, 

0x0500000c, 0x000b4020, 
0xfffff000, //88 Tx's
0xffffffff, 
0xffffffff, 
0x0000000f, 
0xffffffff, //80 Rx's
0xffffffff, 
0x0000ffff, 
0x00000028, //SCM_SSDC (sub-frame start delay count value, based on AFECLK's)   
0x00000028, //SCM_ISDC (integration start delay count value, based on AFECLK's) 
0x00000002, //SCM_IFDC (inter-frame delay count value, based on a 1uS clk)      
0x00000000, //SCM_ADC                                                           
 
0x05000002, 0x0001409c,
0x00000000, 

0x05000002, 0x000140d8, 
0x0000066f,

/*
0x06000029, 0x00504600, 
0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 0x20002000, 0x20002000,
0x20002000, 0x20002000, 

0x03000310, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x56000000, 0x59a6666a,
0x956699a6, 0x69995a69, 0xa6666995, 0x5a969a56,
0x00000066, 0x9a000000, 0x96a5aaaa, 0x56969a65,
0x66965956, 0x5a969656, 0x5a5a9565, 0x000000a9,
0x56000000, 0x569a6aa9, 0xa6a56596, 0x5969999a,
0x96559a5a, 0xa95a5695, 0x0000009a, 0x95000000,
0xaa666966, 0x666569a6, 0x6a59a696, 0x5aa99559,
0x6596996a, 0x00000095, 0x55000000, 0x6aaa5aa6,
0x65a5a655, 0x95566aa6, 0x666a66a5, 0x6a595999,
0x000000a5, 0x69000000, 0x556596a6, 0x95556aa5,
0xa56aa55a, 0x966a556a, 0xa9a99a56, 0x000000a9,
0xa5000000, 0x659665a5, 0x99aa6965, 0xa6a5595a,
0x5aa56aa5, 0xa56a9959, 0x00000096, 0xa5000000,
0x9aa95966, 0x999a6999, 0x66999a6a, 0xa6565659,
0x656566a5, 0x000000a5, 0x99000000, 0x6566a696,
0x9969a69a, 0x5a5a956a, 0x69965959, 0x6a5a6559,
0x000000a6, 0x99000000, 0xa69a9a95, 0x5566699a,
0x6aa95955, 0x55666596, 0xa99969aa, 0x00000069,
0x99000000, 0x599aa655, 0x5aa99995, 0xa669aa55,
0x99aa9669, 0x9a565a95, 0x00000065, 0x66000000,
0x665a6969, 0xa5669a99, 0xa59a9599, 0x5aa6556a,
0x56666999, 0x00000069, 0xa6000000, 0x666a5a5a,
0x9a6999a5, 0x6669656a, 0x6559a596, 0x9a659599,
0x00000099, 0x9a000000, 0x596595aa, 0xaa99a99a,
0xaa595699, 0x5665a6a6, 0x66596955, 0x00000059,
0xa5000000, 0x59956695, 0x66565a99, 0x669aaaa6,
0x6569a99a, 0x596a6955, 0x0000009a, 0x5a000000,
0xaaa5aa5a, 0xa95a6a95, 0xa5aa6a99, 0x55595965,
0xa9565569, 0x00000056, 0xa5000000, 0x9a9659a5,
0x966596aa, 0x9a6a6a59, 0x565a9965, 0x9669a565,
0x00000066, 0xa6000000, 0x95a6655a, 0x69a95696,
0x9a6a59a6, 0x65665666, 0x95a65966, 0x000000a9,
0x6a000000, 0x56a6a9aa, 0x5a6aa559, 0x55a5a965,
0x66699559, 0xa6aa6956, 0x00000055, 0x59000000,
0x6a96a995, 0xaa695969, 0xa5555aaa, 0xa5569566,
0x559aa65a, 0x000000a9, 0xaa000000, 0x56969a59,
0xa5a56a6a, 0x6a5669a9, 0xa9a95655, 0x6965aa55,
0x00000055, 0x9a000000, 0xa599a569, 0x5555a5a9,
0x96596966, 0xa6695966, 0x6656aaa9, 0x0000009a,
0x69000000, 0x9a569a55, 0x96a6a595, 0x9599566a,
0x69a59a66, 0x6aa55966, 0x0000009a, 0xa9000000,
0x999aa665, 0xa969a665, 0x56569a99, 0x5655659a,
0x6aa695a6, 0x00000059, 0x95000000, 0x6a556a65,
0x999a965a, 0x5aa6665a, 0x96695656, 0x9a966a5a,
0x00000099, 0x96000000, 0x5aa6595a, 0x5565aa59,
0xa6669a56, 0x999569aa, 0xa6956656, 0x0000009a,
0x65000000, 0xa9a96596, 0x9655aa56, 0x69665a69,
0xa9a59556, 0xa66a5aa9, 0x00000059, 0x99000000,
0x5a6a99a6, 0x696a56a5, 0x56aa56a5, 0xaaa55956,
0x95599a95, 0x0000005a, 0x5a000000, 0x995aa5a9,
0x95a55a99, 0x656a666a, 0x6a996695, 0x959a65a6,
0x00000095, 0x9a000000, 0x5a59a959, 0x96566666,
0x9aa6966a, 0x6566a5a9, 0xa5569995, 0x000000a5,
0xa9000000, 0x6655a965, 0x6695aa95, 0x666a55a5,
0xa656aa95, 0xa6a6565a, 0x00000066, 0x59000000,
0x96959965, 0xa9599956, 0x6965a9aa, 0x6aaa699a,
0x96555965, 0x000000aa, 0x56000000, 0xa9565669,
0x5aa96a69, 0x65669665, 0x66669a5a, 0xa955a969,
0x0000006a, 0x59000000, 0xa5a69556, 0xa6a696a9,
0x55aa999a, 0x9565a699, 0x9655aa69, 0x00000095,
0x96000000, 0x69955599, 0xa596a5a5, 0x56a9aaa9,
0x699a565a, 0xa699955a, 0x00000069, 0x99000000,
0x65aa9566, 0x96a65a56, 0xaa96a969, 0x66599a6a,
0x5595559a, 0x0000005a, 0xaa000000, 0xa95696a9,
0x65669956, 0xaaa596a6, 0xa6555569, 0x9a695669,
0x00000095, 0x6a000000, 0x69969569, 0x5a6a6696,
0x599a6a56, 0x9a9aa596, 0x65655659, 0x000000a9,
0x55000000, 0x55995665, 0x6a5aaaaa, 0xa9aa59a6,
0x6a959565, 0xaa95a596, 0x00000095, 0x69000000,
0x596aa556, 0x65a6a96a, 0x69a56696, 0x555a5a56,
0xa666a595, 0x000000aa, 0x55000000, 0x96566995,
0x59aaaaa6, 0xa99aa565, 0xa55966aa, 0x665a9665,
0x00000059, 0x69000000, 0x6aa5aa66, 0x559555aa,
0x99699a65, 0x6a9566aa, 0x9966a559, 0x00000059,
0x6a000000, 0x6a999999, 0x59999aa6, 0x695a9a56,
0x55655a59, 0x56a9999a, 0x00000096, 0x66000000,
0xa5669a55, 0x955aaa66, 0x5669aaa6, 0x5695a666,
0x559a6a95, 0x00000066, 0x65000000, 0xa995a96a,
0x55696695, 0x6aa5a59a, 0xa5a69aa6, 0x5a59a596,
0x00000055, 0xa9000000, 0x669665aa, 0x96996556,
0x65aaa6a5, 0x59566565, 0x5a95aaa5, 0x0000009a,
0x65000000, 0xa6a9a5a9, 0x66aa9aa5, 0x5a556669,
0x95956969, 0xa9956955, 0x000000a9, 0xa6000000,
0x5a55a666, 0xa66a9695, 0x69655965, 0x5969666a,
0x6559a6aa, 0x000000a6, 0x66000000, 0x9a9995a6,
0xa5aa5966, 0x966a9555, 0x65aa9596, 0x6a966659,
0x00000096, 0x5a000000, 0xa55659a6, 0x555a9596,
0xa6565aa9, 0x95aaa659, 0xa966a596, 0x0000009a,
0x96000000, 0x95a5aaa5, 0x59a59656, 0xa599a6a9,
0x59659996, 0xa5a5a699, 0x000000a5, 0x96000000,
0x6a9a9666, 0x599596aa, 0x65a56599, 0xa596a56a,
0x65aa5965, 0x00000096, 0xaa000000, 0x95556996,
0x596659a9, 0x59666a99, 0x6a56aa6a, 0x6a955a99,
0x00000095, 0xaa000000, 0x56a95595, 0x956996a9,
0x6995a6a6, 0x56a696a5, 0xa956566a, 0x0000005a,
0xaa000000, 0xa9a96955, 0x9aa666a6, 0xa55555a6,
0x9a6a655a, 0x9a996555, 0x00000056, 0x55000000,
0x9669a669, 0x656965aa, 0xa6a66a6a, 0x5966a659,
0x56a95659, 0x0000005a, 0x95000000, 0x9659955a,
0x5a666696, 0x65559a99, 0x669956a5, 0x9aaaaa59,
0x000000aa, 0xa9000000, 0xa9565aaa, 0x6a9656a9,
0xa965a96a, 0x55a55595, 0x66965695, 0x0000006a,
0x96000000, 0xa9a6a995, 0xa659a5aa, 0x65a65555,
0x66a95aa9, 0x59a55695, 0x000000a9, 0x69000000,
0x5a96655a, 0x695595a6, 0xa696956a, 0x5699aa56,
0xa9996aa9, 0x00000065, 0x96000000, 0x6aa596a5,
0x566a6999, 0x995695aa, 0x559a6a56, 0x96a69aa5,
0x00000055, 0x95000000, 0x6595aa5a, 0xa9a6a9a5,
0x9956a655, 0x66a6a555, 0x956966a6, 0x0000009a,
0x59000000, 0xa69566aa, 0x9a655a6a, 0x56995696,
0x65a9666a, 0xa6655696, 0x00000066, 0x6a000000,
0x9aa56695, 0x6a66a65a, 0xa669a559, 0xa6966655,
0x5555999a, 0x0000009a, 0x99000000, 0xa6a55559,
0x65aa9559, 0x696a965a, 0xaa69a566, 0x69aa9595,
0x00000066, 0xa9000000, 0x55aa6a69, 0x955a5a9a,
0x95659695, 0xa5a9aa55, 0x66999569, 0x000000a9,
0x5a000000, 0x6a655555, 0x56a95aa6, 0x96a5a999,
0x6a656955, 0x6a6a66a6, 0x000000a9, 0x9a000000,
0x69a96965, 0x69665956, 0x556a656a, 0x559596a9,
0x6a69aa66, 0x0000006a, 0xa6000000, 0x9a66a565,
0x59956969, 0x599a99a9, 0x95a9a565, 0x9a59559a,
0x000000aa, 0xa5000000, 0xaa95965a, 0x956a556a,
0xa55a65a5, 0x9a559a99, 0xa65a5996, 0x000000a9,
0x56000000, 0x599aaa96, 0x565a5565, 0x6a5aa59a,
0xaa656665, 0xa6a56569, 0x0000006a, 0xaa000000,
0xaa5956a6, 0xa965a996, 0x5596a956, 0xa965aa65,
0x9596a555, 0x00000069, 0x69000000, 0x69696aa9,
0x55aaa5aa, 0x66569999, 0x695995a6, 0x95555a66,
0x000000a6, 0xa9000000, 0xaa6a6659, 0x56599955,
0x999a6999, 0x66a6569a, 0xa599a655, 0x00000096,
0x9a000000, 0x96666665, 0xa69a55a6, 0x6556aa56,
0xa59659a6, 0xa669699a, 0x00000055, 0x6a000000,
0x66a65665, 0x69566665, 0x6a595659, 0x696a9aa9,
0x969a66a5, 0x00000099, 0x65000000, 0x59659969,
0x9aa6955a, 0x665a6996, 0x65a66969, 0xa99a96a9,
0x00000059, 0x6a000000, 0x55955a56, 0x66a9699a,
0x56966669, 0x96a555aa, 0x969a95a9, 0x000000a6,
0xa5000000, 0x99a69a99, 0x65995a96, 0x5595555a,
0x6a5aa569, 0xa656aa9a, 0x0000005a, 0xaa000000,
0x95995a66, 0x56a9a565, 0xa9aa569a, 0x65996959,
0x555a6a5a, 0x00000069, 0x99000000, 0x596559a9,
0xa5696665, 0xa9996955, 0xa595a65a, 0x9aa669a9,
0x000000a6, 0x59000000, 0x99a55a69, 0x9a6599a5,
0x5aa695a5, 0x995a56a5, 0x65aa6999, 0x0000009a,
0x66000000, 0xa5aa9965, 0xaa555599, 0xaa96a655,
0x69556656, 0xaa5a9a56, 0x000000a6, 0x5a000000,
0xa9596a56, 0xa5a95659, 0x6a999965, 0x569aa965,
0xa6a99a55, 0x00000099, 0x66000000, 0x6665a695,
0x99a55595, 0xaaa65a96, 0xa6999599, 0x6695a9a5,
0x0000006a, 0x5a000000, 0x9a966566, 0x959aaa99,
0x9a556596, 0x69559599, 0x99a99a99, 0x0000006a,
0x59000000, 0x9aaa5699, 0x99966595, 0x6a6665a5,
0x56a5a96a, 0x5666a65a, 0x000000a5, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 

0x04000188,
0x9b928ff1, 0xd700e795, 0xbb86a646, 0x8a0e06b1,
0xe06b1bb2, 0x4e3f28a0, 0x029f7651, 0x089aeb1c,
0x99106bd6, 0x06bfc728, 0xfc728991, 0xe3f29c89,
0x29c89aeb, 0xf1aebe3f, 0x959b928f, 0xb928ff17,
0xae017959, 0x1b0c4cb5, 0xc4cb5ae0, 0xe53913b0,
0x9135d7c7, 0x5d7c7e53, 0xfc5e5191, 0x1ae6eb5d,
0xc34a203a, 0xfe2944ca, 0x6bb353c1, 0x6561e01d,
0x18ff14a2, 0xf14a2656, 0x06b1bb8f, 0xe4428a0e,
0xd75f1f94, 0xca41c145, 0xe3ebac89, 0xd737635b,
0x6f86bfc2, 0x6bfc2d76, 0x9c8966f8, 0x6aebe3f2,
0x41c0d637, 0x80725151, 0x251322f2, 0x322f2807,
0x2944cac3, 0x4cac31fe, 0xcd1fe294, 0x95806c36,
0x06c36cd2, 0xc94f2958, 0x038175ae, 0xe8a5136b,
0x38faebc7, 0x514dd8d6, 0x22729c70, 0xb5d9b9e5,
0x6461a00e, 0xffff14a2, 0x79d98528, 0x98528ffa,
0xdf29a79d, 0x6071ae8d, 0xe517220d, 0x3800a5f8,
0x0a51334f, 0x1334f380, 0x00d86d9b, 0x79264529,
0x29ad6ff2, 0x4c53ce02, 0xc31fdd64, 0xb807bb3a,
0x8c353216, 0xacde947f, 0x3585d93c, 0xa2645270,
0xbad8fca7, 0xad766a79, 0x99586c03, 0x7f3fc528,
0x3cece294, 0x31d6b805, 0xc145ac31, 0x5c89ca41,
0x9ca41c14, 0x5bb653c8, 0x6530e15d, 0x0e15d5bb,
0x376517e3, 0x517e3f29, 0x7ff29376, 0x3cece294,
0xce2947f5, 0x2fc753ce, 0x7e526eca, 0x26eca2fc,
0x4c8c35e5, 0x3cafe294, 0x01d6bb37, 0x992b5c03,
0x4a3fc61a, 0x359e566d, 0xe2944c8c, 0x44c8c35f,
0x5cddfe29, 0x29440703, 0xbf8fca32, 0xca2fc76b,
0x038e526e, 0x1ad9135d, 0x806c36cd, 0x3c932295,
0x51d6a7f9, 0x22f28072, 0xd7f8dae3, 0x578fcdd0,
0x0ca6dd94, 0x9226ba87, 0xf764a735, 0xb581c0ba,
0xe3f45089, 0x00d86d9b, 0x86d9b529, 0xa552900d,
0x334f3800, 0x0c7ff5a1, 0xd337eccb, 0x8a5001f0,
0xffe0f2cc, 0x766a795a, 0xa79c03ad, 0xc03ad766,
0x2899106b, 0x6ef94fc7, 0xe5038d76, 0x0eb5d9b9,
0x5d9b9e50, 0x022900eb, 0x944c53ce, 0xc53ce022,
0x28a71f44, 0x71f94fbb, 0x94fbb28a, 0xe02b0bb3,
0x4f44c8a4, 0xa775b1fd, 0xc0baf764, 0xaf764a71,
0xa71f1c0b, 0x094fbb28, 0x044d758e, 0xff1b5d6b,
0xb5d9ba1a, 0x9ba1aff1, 0x5d5bb653, 0x449ac0e1,
0x75f1ea2a, 0xec6b3c7d, 0xb3c7d756, 0x1b5d6ec6,
0x29ba1aff, 0x5e500e4a, 0x6517e364, 0x81cf2937,
0x0d6c89ae, 0x3fe9e099, 0x9e099eb4, 0x8feb43fe,
0xa6dd9457, 0xd94578fc, 0x230dca6d, 0xd7f8a513,
0x8a513230, 0x5fe56b7f, 0xa94b36c3, 0x4c93ca01,
0x4cb5ae01, 0xa51feb0c, 0x6b4f3b34, 0x36c35fe5,
0x35fe56bb, 0x5cddb36c, 0xd9440703, 0x407035cd,
0x8ff6b594, 0x6b5d9b61, 0xd9b618ff, 0x5863fbad,
0x3fbad767, 0x4f767586, 0x8175aec9, 0x5aec94f3,
0xfaeb3817, 0xbdd8d638, 0x8d638fae, 0xbfc2d7dd,
0xd2866f86, 0x99079403, 0x618ff6b5, 0x0094ad9b,
0x0d2649e7, 0xf8a51323, 0xaecdcf27, 0x28ff8075,
0x0a79d985, 0x6267ad70, 0x8ddf2958, 0xf29071ae,
0x071ae8dd, 0x14a26561, 0xd9a9e8ff, 0x13700eb5,
0xd7c7e539, 0x81ac6ec5, 0x4e3fa283, 0xf29f7651,
0xf76514e3, 0x63fbad29, 0x45276758, 0x898a79c0,
0x703585d9, 0x585d9452, 0x91452703, 0x5dfc5e51,
0xc5e5191b, 0xb807b5df, 0x7c353216, 0x53216b80,
0xf0d337c3, 0x337a5001, 0xa5001f0d, 0x6b3c7d75,
0x3828a6ec, 0xb391394c, 0xa4e02b0b, 0x02b0bb38,
0xe0998a4e, 0x6eb43fe9, 0xbc0161f6, 0x2b5c0314,
0xc0361a99, 0x61a992b5, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
0xffffffff, 0xffffffff, 0xffffffff, 0xffffffff,
*/
0x05000002, 0x00011804, 
0x0000002c, //was 220,   0x20: FW_BLSO set, 0xc: disable CDM and FW_BL

0x05000003, 0x0002400c, 
0x00000002, 
0x000001e0, //was 1e0

0x06000029, 0x00504600, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 0x20202020, 0x20202020,
0x20202020, 0x20202020, 

0x03000310, 
0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x04000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x10000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x40000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000010, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000040, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000400, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00001000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00004000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00040000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00100000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00400000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x04000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x10000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x40000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000010, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000040, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000100, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000400, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00001000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00004000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00040000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00100000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00400000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x04000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x10000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x40000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000010, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000100, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000400, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00001000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00004000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00040000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00100000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00400000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x01000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x04000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x10000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x40000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000100, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000400, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00001000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00004000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00040000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00400000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x10000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x40000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000100, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000400, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00001000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00004000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00010000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00040000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00100000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00400000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x01000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x04000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x10000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x40000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000001, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000004, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000010, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000040, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 
0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 0x00000000, 

0x04000188,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,
0x00000000, 0x00000000, 0x00000000, 0x00000000,

/*
//WOE setup
0x05000002, 0x00014940, 
0x00004641, 

0x05000002, 0x00014948, 
0x00006e50, 

0x05000002, 0x00014950, 
0x00000821, 

0x05000002, 0x00014644, 
0x00002004, 

0x05000002, 0x0001464c, 
0x00002004, 

0x05000002, 0x00014658, 
0x00002001, 

0x05000002, 0x00014664, 
0x00002004, 

0x05000002, 0x0001466c, 
0x00002004, 

0x05000002, 0x000146d4, 
0x0000200c, 

0x05000002, 0x000146dc, 
0x0000200c, 

0x05000002, 0x000146e8, 
0x00002002, 

0x05000002, 0x000146f4, 
0x0000200c, 

0x05000002, 0x000146fc, 
0x0000200c, 

0x05000004, 0x00030380, 
0x000087d0, 
0x01401004, 
0x00000ee8, 

0x05000004, 0x00030394, 
0x00000001, 
0x00000010, 
0x00074074, 

0x05000003, 0x000203a4, 
0x000044ee, 
0x11111111, 
*/

};
