const char *REPORT_Head="<!DOCTYPE html PUBLIC \"-//W3C//DTD XHTML 1.0 Transitional//EN\" \"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd\"><html><head><meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB2312\"><title>�ʷ���������</title><script>var x,y;function showTip(s) {x = document.body.scrollLeft+event.clientX;y = document.body.scrollTop+event.clientY;if(s==null) {s=\"\";x=-24;y=-24;}var t=document.getElementById(\"tip\");t.style.posLeft=x+10;t.style.posTop=y+6;t.innerHTML=s;}</script></head><body style=\"width:94%; font-family:Consolas,΢���ź�; padding:20px; font-size:14px;\">";

const char *REPORT_CodeHead="<span style=\"float:right;\"></span><ul><li>����Դ�ļ���";
const char *REPORT_CodeAfter="</li><li>����������<span id=\"line\"></span></li><li>���ʸ�����<span id=\"word\"></span></li><li>ע��������<span id=\"note\"></span></li><li>���������<span id=\"error\"></span></li><br/><li>������������ڴ������ƶ���������ʾ��ϸ˵����</li></ul><table cellspacing=\"0\" cellpadding=\"3\" style=\"border:solid 1px #66CCFF;\">";
const char *REPORT_LineHead1="<tr><td style=\"vertical-align:top;text-align:right;width:40px;border-right:solid 1px #66CCFF;\">";
const char *REPORT_LineHead2="<tr style=\"background-color:#EAFAFF;\"><td style=\"vertical-align:top;text-align:right;width:40px;border-right:solid 1px #66CCFF;\">";
const char *REPORT_LineAfter="</td><td style=\"padding-rignt:2em;padding-left:2em;text-indent:-1em;\">";
const char *REPORT_TipHead="<span onmouseout=\"showTip()\" onmousemove=\"showTip('";
const char *REPORT_TipAfter="');\" style=\"color:red;\">";
const char *REPORT_WordAfter="</span>&nbsp;";
const char *REPORT_LineEnd="</td></tr>";
const char *REPORT_CodeEnd="</table><br/><span style=\"float:right;\">&lt;THANKS&gt;</span><br/><script>";
const char *REPORT_End="</script><div id=\"tip\" style=\"position:absolute;left:-24px;top:-24px;background-color:#88AAFF;border:solid 1px #0000FF\"></div></body></html>";
