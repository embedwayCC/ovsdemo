#!/usr/bin/python

#Usage python bnf_parser.py -i input_file [-l lookup_file] [-p parser_file] [-a ahelp_file]

import sys
import getopt
import string
import re

token_list = [[] for x in range(26)]
var_list = [[] for x in range(50)]
help_row_list = [[] for x in range(512)]
help_col_list = [[] for x in range(64)]

def parser_argument(argv):
    try:
        opts, args = getopt.getopt(argv, "i:l:p:a:", ["input_file", "lookup_file", "parser_file", "ahelp_file"])
    except getopt.GetoptError:
        return None

    input_file = ""
    lookup_file = ""
    parser_file = ""
    ahelp_file = ""
    for opt, arg in opts:
        if opt in ["-i", "--input_file"]:
            input_file = arg
        elif opt in ["-l", "--lookup_file"]:
            lookup_file = arg
        elif opt in ["-p", "--parser_file"]:
            parser_file = arg
        elif opt in ["-a", "--ahelp_file"]:
            ahelp_file = arg
    if input_file == "":
        return None
    return (input_file, lookup_file, parser_file, ahelp_file)

def get_pure_word(pure_str, set_token, set_var):
    index = ord(pure_str[0]) - ord('a')
    if set_token and pure_str not in token_list[index]:
        token_list[index].append(pure_str)
    if set_var and pure_str not in var_list[index]:
        var_list[index].append(pure_str)

def parse_angle_quote(str):
    #print "token:%s is in angle quote" % (str)
    cut_str = str[1:len(str)-1]
    #print cut_str
    if cut_str.isalpha():
        get_pure_word(cut_str, 1, 1)
    else:
        get_pure_word(cut_str, 0, 1)

def parse_square_bracket(str):
    #print "token:%s is in square_bracket" % (str)
    cut_str = str[1:len(str)-1]
    #print cut_str
    if cut_str.isalpha():
        get_pure_word(cut_str, 1, 0)
    elif cut_str.find('=') != -1:
        #print "find = "
        split_str = cut_str.split('=')
        #print "with '=', split_str:", split_str
        get_pure_word(split_str[0], 1, 0)
        parse_square_bracket(split_str[1])
    elif cut_str.find('|') != -1:
        #print "find |"
        split_str = cut_str.split('|')
        for i in range(len(split_str)):
            sub_str = split_str[i]
            #print "sub_str:", sub_str
            if sub_str.isalpha():
                get_pure_word(sub_str, 1, 1)
            elif sub_str[0] == '<':
                var_str = re.findall('<[\w|_]*>', sub_str)
                #print "var_str:", var_str
                for j in range(len(var_str)):
                    parse_angle_quote(var_str[j])

def parser_input_file(input_file):
    fin = open(input_file, "r")
    textlist = fin.readlines()
    line_index = 0
    for line in textlist:
        strip_str = line.strip()
        if strip_str:
            #print strip_str
            split_str = strip_str.split(" ")
            #print split_str
            #help_row_list[line_index].append(str(len(split_str)))
            help_row_list[line_index] = help_row_list[line_index] + split_str
            index = 0
            for i in range(len(split_str)):
                if split_str[i].isalpha():
                    if split_str[i] not in help_col_list[i]:
                        help_col_list[i].append(split_str[i])
                    index = ord(split_str[i][0]) - ord('a')
                    if split_str[i] not in token_list[index]:
                        token_list[index].append(split_str[i])
                elif split_str[i][0] in ['[', '{']:
                    #print "token:%s" % (split_str[i])
                    parse_square_bracket(split_str[i])
                elif split_str[i][0] == '<':
                    #print "token:%s" % (split_str[i])
                    parse_angle_quote(split_str[i])
            line_index += 1

    fin.close()

def format_print(fout, token, match_len):
    blank_num = 20 - len(token)
    if fout is not None:
        fout.write('\n    { "%s",%s%s,%s%d },' % (token, ' '*blank_num, string.upper(token), ' '*blank_num, match_len))

def gen_lookup_parser_file(lookup_file, parser_file):
    if lookup_file != "":
        fout_lookup = open(lookup_file, "w")
        lookup_header = lookup_file + ".header"
        #print lookup_header
        flookup_header_in = open(lookup_header, "r")
        lookup_header_list = flookup_header_in.readlines()
        for lookup_header_line in lookup_header_list:
            fout_lookup.write('%s' % lookup_header_line)
        flookup_header_in.close()
    else:
        fout_lookup = None

    if parser_file != "" :
        fout_parser = open(parser_file, "w")
        parser_header = parser_file + ".header"
        #print parser_header
        fparser_header_in = open(parser_header, "r")
        parser_header_list = fparser_header_in.readlines()
        for parser_header_line in parser_header_list:
            fout_parser.write('%s' % parser_header_line)
        fparser_header_in.close()
    else:
        fout_parser = None

    if fout_lookup is not None:
        fout_lookup.write("token_ent token_table[] = {\n")

    for j in range(26):
        if j == (ord('a') - ord('a')):
            token_list[j].append("a9")
            token_list[j].append("ac1200")
            token_list[j].append("ack")
            token_list[j].append("aging_time")
        elif j == (ord('b') - ord('a')):
            token_list[j].append("b")
        elif j == (ord('c') - ord('a')):
            token_list[j].append("change_ttl")
        elif j == (ord('d') - ord('a')):
            token_list[j].append("d")
            token_list[j].append("default_pvlan")
            token_list[j].append("device")
            token_list[j].append("dip")
            token_list[j].append("dport")
            token_list[j].append("dst_ip")
            token_list[j].append("dst_mac")
            token_list[j].append("dmac")
            token_list[j].append("dual_star")
            token_list[j].append("dual_dual_star")
        elif j == (ord('e') - ord('a')):
            token_list[j].append("eos")
            token_list[j].append("ether")
            token_list[j].append("ethertype")
        elif j == (ord('f') - ord('a')):
            token_list[j].append("f")
            token_list[j].append("fcs_rate")
            token_list[j].append("five_tuple")
            token_list[j].append("fin")
            token_list[j].append("fr_16xe_fb_11qxe_2dxe")
            token_list[j].append("fr_20xe_fb_9qxe_4dxe")
            token_list[j].append("fr_24xe_fb_8qxe_4dxe")
        elif j == (ord('g') - ord('a')):
            token_list[j].append("grule")
        elif j == (ord('h') - ord('a')):
            token_list[j].append("head")
            token_list[j].append("hostip")
        elif j == (ord('i') - ord('a')):
            token_list[j].append("icmp")
            token_list[j].append("innervlan")
            token_list[j].append("invert")
            token_list[j].append("ipv6")
            token_list[j].append("ip_route_static")
            token_list[j].append("ipv6_route_static")
        elif j == (ord('l') - ord('a')):
            token_list[j].append("l2cmds")
            token_list[j].append("l3cmds")
            token_list[j].append("l3packetlen")
            token_list[j].append("l2")
            token_list[j].append("l3")
            token_list[j].append("l4")
            token_list[j].append("link_protect")
            token_list[j].append("load_balance")
        elif j == (ord('m') - ord('a')):
            token_list[j].append("m")
            token_list[j].append("mac_address")
            token_list[j].append("mac_ignore")
            token_list[j].append("md5")
            token_list[j].append("mode_route")
        elif j == (ord('n') - ord('a')):
            #token_list[j].append("name")
            token_list[j].append("none")
            token_list[j].append("no_vlan")
            token_list[j].append("no_shutdown")
        elif j == (ord('o') - ord('a')):
            token_list[j].append("olp")
            token_list[j].append("output_interface")
        elif j == (ord('p') - ord('a')):
            token_list[j].append("protocol")
            token_list[j].append("psh")
        elif j == (ord('r') - ord('a')):
            token_list[j].append("r")
            token_list[j].append("rst")
            token_list[j].append("ruleset")
            token_list[j].append("route_static")
            token_list[j].append("routing_table")
            token_list[j].append("rx")
            token_list[j].append("rx_fcs")
            token_list[j].append("rx_fcs_rate")
            token_list[j].append("rx_rate")
            token_list[j].append("rx_tx_percent")
            token_list[j].append("rx_vlantag")
        elif j == (ord('s') - ord('a')):
            token_list[j].append("s")
            token_list[j].append("s40")
            token_list[j].append("s320")
            token_list[j].append("scan_frequency")
            token_list[j].append("server")
            token_list[j].append("single_fiber")
            token_list[j].append("size")
            token_list[j].append("sport")
            token_list[j].append("src_dst_ip")
            token_list[j].append("src_dst_mac")
            token_list[j].append("src_ip")
            token_list[j].append("src_mac")
            token_list[j].append("st")
            token_list[j].append("smac")
            token_list[j].append("soc")
            token_list[j].append("syn")
        elif j == (ord('t') - ord('a')):
            token_list[j].append("tcp")
            token_list[j].append("tcpflag")
            token_list[j].append("tx")            
            token_list[j].append("tx_fcs")
            token_list[j].append("tx_fcs_rate")
            token_list[j].append("tx_rate")            
            token_list[j].append("tx_rx_percent")            
        elif j == (ord('u') - ord('a')):
            token_list[j].append("ud0")
            token_list[j].append("ud1")
            token_list[j].append("ud2")
            token_list[j].append("ud3")
            token_list[j].append("ud4")
            token_list[j].append("ud5")
            token_list[j].append("ud6")
            token_list[j].append("ud7")
            token_list[j].append("udp")
            token_list[j].append("urg")
        elif j == (ord('v') - ord('a')):
            token_list[j].append("vlan_filter")
            token_list[j].append("vlan_mapping")
            token_list[j].append("vlan_s")
        elif j == (ord('w') - ord('a')):
            token_list[j].append("weight")
        if token_list[j]:
            sorted_token_list = sorted(token_list[j])
            #print sorted_token_list
            #print "sorted_token_len:", len(sorted_token_list)
            match_len = 0
            if fout_parser is not None:
                fout_parser.write('%token')
            for k in range(len(sorted_token_list)):
                next_k = k + 1
                #print "k:%d next_k:%d" % (k, next_k)
                if next_k < len(sorted_token_list):
                    token = sorted_token_list[k]
                    next_token = sorted_token_list[next_k]
                    cur_token_len = len(token)
                    next_token_len = len(next_token)
                    index = 0
                    while (index < cur_token_len and index < next_token_len and token[index] == next_token[index]):
                        index += 1
                    #print "token_len:%d next_token_len:%d index:%d match_len:%d" % (cur_token_len, next_token_len, index, match_len)
                    if match_len > index:
                        format_print(fout_lookup, token, match_len)
                        match_len = index + 1
                    else:
                        if index == cur_token_len:
                            match_len = index
                        else:
                            match_len = index + 1
                        format_print(fout_lookup, token, match_len)
                        if index == cur_token_len:
                            match_len += 1
                else:
                    if match_len == 0:
                        match_len = match_len + 1
                    format_print(fout_lookup, sorted_token_list[k], match_len)

                if fout_parser is not None:
                    fout_parser.write(' %s' % (string.upper(sorted_token_list[k])))
            if fout_parser is not None:
                fout_parser.write('\n')

    if fout_lookup is not None:
        fout_lookup.write('\n    { NULL,%s0,%s0 }' % (' '*18, ' '*19))
    if fout_lookup is not None:
        fout_lookup.write('\n};\n')

    if fout_lookup is not None:
        lookup_footer = lookup_file + ".footer"
        #print lookup_footer
        flookup_footer_in = open(lookup_footer, "r")
        lookup_footer_list = flookup_footer_in.readlines()
        for lookup_footer_line in lookup_footer_list:
            fout_lookup.write('%s' % lookup_footer_line)
        flookup_footer_in.close()
        fout_lookup.close()

    if fout_parser is not None:
        parser_footer = parser_file + ".footer"
        #print parser_footer
        fparser_footer_in = open(parser_footer, "r")
        parser_footer_list = fparser_footer_in.readlines()
        for parser_footer_line in parser_footer_list:
            fout_parser.write('%s' % parser_footer_line)
        fparser_footer_in.close()
        fout_parser.close()

def gen_ahelp_file(ahelp_file):
    if ahelp_file != "":
        fout_ahelp = open(ahelp_file, "w")
    else:
        fout_ahelp = None

    if fout_ahelp is not None:
        fout_ahelp.write('#include "my_parser.h"\n\n')
        fout_ahelp.write('char *helplist[MAX_CMD_NUM][MAX_CMD_WORD_NUM] = {\n')

    cmd_len = 0
    for m in range(512):
        if help_row_list[m]:
            #print help_row_list[m]
            fout_ahelp.write('\t{')
            list = help_row_list[m]
            list_len = len(list)
            if list_len > cmd_len:
                cmd_len = list_len
            fout_ahelp.write('"%d"' % list_len)
            for num in range(list_len):
                if list[num].find('"') == -1:
                    fout_ahelp.write(', "%s"' % (list[num]))
                else:
                    replace_str = list[num].replace(r'"', r'\"')
                    #print replace_str
                    fout_ahelp.write(', "%s"' % replace_str)
            if fout_ahelp is not None:
                fout_ahelp.write('},\n')
    if fout_ahelp is not None:
        fout_ahelp.write('};\n\n')

    if fout_ahelp is not None:
        fout_ahelp.write('char *tokenlist[MAX_CMD_WORD_NUM][MAX_CMD_NUM] = {\n')
    for k in range(cmd_len):
        #if help_col_list[k]:
            #print help_col_list[k]
            fout_ahelp.write('\t{')
            list = help_col_list[k]
            list_len = len(list)
            fout_ahelp.write('"%d"' % list_len)
            for num in range(list_len):
                if fout_ahelp is not None:
                    fout_ahelp.write(', "%s"' % (list[num]))
            if fout_ahelp is not None:
                fout_ahelp.write('},\n')
    if fout_ahelp is not None:
        fout_ahelp.write('};\n')

    if fout_ahelp is not None:
        fout_ahelp.close()

if __name__ == '__main__':
    input_file, lookup_file, parser_file, ahelp_file = parser_argument(sys.argv[1:])
    #print input_file
    if input_file != "":
        parser_input_file(input_file)
    else:
        print "No input file"
        exit -1
    #print lookup_file
    #print parser_file
    if lookup_file != "" or parser_file != "":
        gen_lookup_parser_file(lookup_file, parser_file)
    #print ahelp_file
    if ahelp_file != "":
        gen_ahelp_file(ahelp_file)
