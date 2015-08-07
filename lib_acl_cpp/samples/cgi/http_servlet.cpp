// http_servlet.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

using namespace acl;

//////////////////////////////////////////////////////////////////////////

class http_servlet : public HttpServlet
{
public:
	http_servlet(void)
	{

	}

	~http_servlet(void)
	{

	}

	virtual bool doUnknown(HttpServletRequest&, HttpServletResponse& res)
	{
		res.setStatus(400);
		res.setContentType("text/xml; charset=gb2312");
		// ���� http ��Ӧͷ
		if (res.sendHeader() == false)
			return false;
		// ���� http ��Ӧ��
		string buf("<root error='unkown request method' />\r\n");
		(void) res.getOutputStream().write(buf);
		return false;
	}

	virtual bool doGet(HttpServletRequest& req, HttpServletResponse& res)
	{
		return doPost(req, res);
	}

	virtual bool doPost(HttpServletRequest& req, HttpServletResponse& res)
	{
		const char* sid = req.getSession().getAttribute("sid");
		if (*sid == 0)
			req.getSession().setAttribute("sid", "xxxxxx");
		sid = req.getSession().getAttribute("sid");

		const char* cookie1 = req.getCookieValue("name1");
		const char* cookie2 = req.getCookieValue("name2");

		// ���� HTTP ��Ӧͷ
                res.addCookie("name1", "value1");
		res.addCookie("name2", "value2", ".test.com", "/", 3600 * 24);
//		res.setStatus(400);  // �������÷��ص�״̬��

		// ���ַ�ʽ�����������ַ���
		if (0)
			res.setContentType("text/xml; charset=gb2312");
		else
		{
			res.setContentType("text/xml");
			res.setCharacterEncoding("gb2312");
		}

		const char* param1 = req.getParameter("name1");
		const char* param2 = req.getParameter("name2");

		// ���� xml ��ʽ��������
		xml body;
		body.get_root().add_child("root", true)
			.add_child("sessions", true)
				.add_child("session", true)
					.add_attr("sid", sid ? sid : "null")
					.get_parent()
				.get_parent()
			.add_child("cookies", true)
				.add_child("cookie", true)
					.add_attr("name1", cookie1 ? cookie1 : "null")
					.get_parent()
				.add_child("cookie", true)
					.add_attr("name2", cookie2 ? cookie2 : "null")
					.get_parent()
				.get_parent()
			.add_child("params", true)
				.add_child("param", true)
					.add_attr("name1", param1 ? param1 : "null")
					.get_parent()
				.add_child("param", true)
					.add_attr("name2", param2 ? param2 : "null");
		string buf;
		body.build_xml(buf);

		// ���� http ��Ӧͷ
		if (res.sendHeader() == false)
			return false;
		// ���� http ��Ӧ��
		if (res.getOutputStream().write(buf) == -1)
			return false;
		return true;
	}
protected:
private:
};

//////////////////////////////////////////////////////////////////////////

static void do_run(socket_stream* stream)
{
	memcache_session session("127.0.0.1:11211");
	http_servlet servlet;
	servlet.setLocalCharset("gb2312");
	servlet.doRun(session, stream);
}

// ��������ʽ����ʱ�ķ�����
class master_service : public master_proc
{
public:
	master_service() {}
	~master_service() {}
protected:
	virtual void on_accept(socket_stream* stream)
	{
		do_run(stream);
	}
};

// WEB ����ģʽ
static void do_alone(void)
{
	master_service service;
	printf("listen: 0.0.0.0:8888 ...\r\n");
	service.run_alone("0.0.0.0:8888", NULL, 0);  // �������з�ʽ
}

// WEB CGI ģʽ
static void do_cgi(void)
{
	do_run(NULL);
}

int main(int argc, char* argv[])
{
#ifdef WIN32
	acl::acl_cpp_init();
#endif

	// ��ʼ����
	if (argc >= 2 && strcmp(argv[1], "alone") == 0)
		do_alone();
	else
		do_cgi();

	return 0;
}
