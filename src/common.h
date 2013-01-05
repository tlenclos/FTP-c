void error(const char *msg);

// Commandes FTP selon la RFC
char const *commandes[33] = {
	// Controle d'accès
	"USER",
	"PASS",
	"ACCT",
	"CWD",
	"CDUP",
	"SMNT",
	"REIN",
	"QUIT",
	// Paramètres de tranfert
	"PORT",
	"PASV",
	"TYPE",
	"STRU",
	"MODE",
	// Service FTP
	"RETR",
	"STOR",
	"STOU",
	"APPE",
	"ALLO",
	"REST",
	"RNFR",
	"RNTO",
	"ABOR",
	"DELE",
	"RMD",
	"MKD",
	"PWD",
	"LIST",
	"NLST",
	"SITE",
	"SYST",
	"STAT",
	"HELP",
	"NOOP"
};
