/*********************************************************************************
	Copyright 2007 Dancefire (dancefire@gmail.com).
	All rights reserved.

	Redistribution and use in source and binary forms, with or without
	modification, are permitted provided that the following conditions
	are met:

	1. Redistributions of source code must retain the above copyright
	   notice, this list of conditions and the following disclaimer.
	2. Redistributions in binary form must reproduce the above copyright
	   notice, this list of conditions and the following disclaimer in the
	   documentation and/or other materials provided with the distribution.

	THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
	ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
	ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
	FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
	OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
	HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
	LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
	OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
	SUCH DAMAGE.


	��Ȩ���� 2007 Dancefire (dancefire@gmail.com)��
	��������Ȩ����

	����������������ǰ���£��������·ַ��޸Ĺ���δ���޸ĵģ���Դ������ѱ�����ʽ
	���ڵı������

	1. ��Դ������ʽ���·������뱣��δ���޸ĵ�������Ȩ����������������Լ�������
	   ��������
	2. ���ѱ�����ʽ���·��������ڷ����汾���ĵ���/������ͬʱ��������������������
	   ��Ȩ����������������Լ���������������

	����������߼������� ������״��ʽ��(AS-IS) �ṩ���ڴ���ʾ���ṩ�κΰ�����������
	�����ض�Ŀ�ĵ������ԡ�ʵ���Ե���ʾ��ʾ�ĵ��������κ�����£����߼������߻���
	�������߽Բ�������ʹ�ô������ɵ�ֱ�ӡ���ӡ��������ر𡢳ͽ�Ļ������ɵĵ�
	��(���������޶��ڻ������Ｐ�����޷�ʹ�á����ݶ�ʧ����ʧӯ���������ж�)��
	�����������κ������ʹ��ʱ�������������ɵġ����ں��������ƶϡ��Ƿ����ں�
	ͬ���롢�ϸ��⳥���λ�������Ȩ��Ϊ(�������������ԭ��)����ʹԤ�ȱ���֪������
	�����Ŀ����ԡ�
	
**********************************************************************************/
/*
*	$Date$
*	$Revision$
*	$Author$
*	$Id$
*/

namespace ictclas{

	const wchar_t* PKU_TAG_STRING[] = 
	{
		L"ag",	//	������	���ݴ������ء����ݴʴ���Ϊa�����ش����ǰ������A��
		L"a",	//	���ݴ�	ȡӢ�����ݴ�adjective�ĵ�1����ĸ��
		L"ad",	//	���δ�	ֱ����״������ݴʡ����ݴʴ���a�͸��ʴ���d����һ��
		L"an",	//	���δ�	�������ʹ��ܵ����ݴʡ����ݴʴ���a�����ʴ���n����һ��
		L"b",	//	�����	ȡ���֡��𡱵���ĸ��
		L"c",	//	����		ȡӢ������conjunction�ĵ�1����ĸ��
		L"dg",	//	������	���������ء����ʴ���Ϊd�����ش����ǰ������D��
		L"d",	//	����		ȡadverb�ĵ�2����ĸ�������1����ĸ���������ݴʡ�
		L"e",	//	̾��		ȡӢ��̾��exclamation�ĵ�1����ĸ��
		L"f",	//	��λ��	ȡ���֡����� ����ĸ��
		L"g",	//	����		����������ض�����Ϊ�ϳɴʵġ��ʸ�����ȡ���֡���������ĸ��
		L"h",	//	ǰ�ӳɷ�	ȡӢ��head�ĵ�1����ĸ��
		L"i",	//	����		ȡӢ�����idiom�ĵ�1����ĸ��
		L"j",	//	�������	ȡ���֡��򡱵���ĸ��
		L"k",	//	��ӳɷ�
		L"l",	//	ϰ����	ϰ������δ��Ϊ����е㡰��ʱ�ԡ���ȡ���١�����ĸ��
		L"m",	//	����		ȡӢ��numeral�ĵ�3����ĸ��n��u�������á�
		L"ng",	//	������	���������ء����ʴ���Ϊn�����ش����ǰ������N��
		L"n",	//	����		ȡӢ������noun�ĵ�1����ĸ��
		L"nr",	//	����		���ʴ���n�͡���(ren)������ĸ����һ��
		L"ns",	//	����		���ʴ���n�ʹ����ʴ���s����һ��
		L"nt",	//	��������	���š�����ĸΪt�����ʴ���n��t����һ��
		L"nz",	//	����ר��	��ר������ĸ�ĵ�1����ĸΪz�����ʴ���n��z����һ�� 
		L"o",	//	 ������	ȡӢ��������onomatopoeia�ĵ�1����ĸ��
		L"p",	//	���		ȡӢ����prepositional�ĵ�1����ĸ��
		L"q",	//	����		ȡӢ��quantity�ĵ�1����ĸ��
		L"r",	//	����		ȡӢ�����pronoun�ĵ�2����ĸ,��p�����ڽ�ʡ�
		L"s",	//	������	ȡӢ��space�ĵ�1����ĸ��
		L"tg",	//	ʱ����	ʱ��������ء�ʱ��ʴ���Ϊt,�����صĴ���gǰ������T��
		L"t",	//	ʱ���	ȡӢ��time�ĵ�1����ĸ��
		L"u",	//	����		ȡӢ������auxiliary �ĵ�2����ĸ,��a���������ݴʡ�
		L"vg",	//	������	���������ء����ʴ���Ϊv�������صĴ���gǰ������V��
		L"v",	//	����		ȡӢ�ﶯ��verb�ĵ�һ����ĸ��
		L"vd",	//	������	ֱ����״��Ķ��ʡ����ʺ͸��ʵĴ��벢��һ��
		L"vn",	//	������	ָ�������ʹ��ܵĶ��ʡ����ʺ����ʵĴ��벢��һ��
		L"w",	//	������   
		L"x",	//	��������	��������ֻ��һ�����ţ���ĸxͨ�����ڴ���δ֪�������š�
		L"y",	//	������	ȡ���֡������ĸ��
		L"z"		//	״̬��	ȡ���֡�״������ĸ��ǰһ����ĸ��
	};
	class TaggingConstants{
		enum PKU_Tag_Index{
		    ag,	//	������	���ݴ������ء����ݴʴ���Ϊa�����ش����ǰ������A��
		    a,	//	���ݴ�	ȡӢ�����ݴ�adjective�ĵ�1����ĸ��
		    ad,	//	���δ�	ֱ����״������ݴʡ����ݴʴ���a�͸��ʴ���d����һ��
		    an,	//	���δ�	�������ʹ��ܵ����ݴʡ����ݴʴ���a�����ʴ���n����һ��
		    b,	//	�����	ȡ���֡��𡱵���ĸ��
		    c,	//	����		ȡӢ������conjunction�ĵ�1����ĸ��
		    dg,	//	������	���������ء����ʴ���Ϊd�����ش����ǰ������D��
		    d,	//	����		ȡadverb�ĵ�2����ĸ�������1����ĸ���������ݴʡ�
		    e,	//	̾��		ȡӢ��̾��exclamation�ĵ�1����ĸ��
		    f,	//	��λ��	ȡ���֡����� ����ĸ��
		    g,	//	����		����������ض�����Ϊ�ϳɴʵġ��ʸ�����ȡ���֡���������ĸ��
		    h,	//	ǰ�ӳɷ�	ȡӢ��head�ĵ�1����ĸ��
		    i,	//	����		ȡӢ�����idiom�ĵ�1����ĸ��
		    j,	//	�������	ȡ���֡��򡱵���ĸ��
		    k,	//	��ӳɷ�
		    l,	//	ϰ����	ϰ������δ��Ϊ����е㡰��ʱ�ԡ���ȡ���١�����ĸ��
		    m,	//	����		ȡӢ��numeral�ĵ�3����ĸ��n��u�������á�
		    ng,	//	������	���������ء����ʴ���Ϊn�����ش����ǰ������N��
		    n,	//	����		ȡӢ������noun�ĵ�1����ĸ��
		    nr,	//	����		���ʴ���n�͡���(ren)������ĸ����һ��
		    ns,	//	����		���ʴ���n�ʹ����ʴ���s����һ��
		    nt,	//	��������	���š�����ĸΪt�����ʴ���n��t����һ��
		    nz,	//	����ר��	��ר������ĸ�ĵ�1����ĸΪz�����ʴ���n��z����һ�� 
		    o,	//	 ������	ȡӢ��������onomatopoeia�ĵ�1����ĸ��
		    p,	//	���		ȡӢ����prepositional�ĵ�1����ĸ��
		    q,	//	����		ȡӢ��quantity�ĵ�1����ĸ��
		    r,	//	����		ȡӢ�����pronoun�ĵ�2����ĸ,��p�����ڽ�ʡ�
		    s,	//	������	ȡӢ��space�ĵ�1����ĸ��
		    tg,	//	ʱ����	ʱ��������ء�ʱ��ʴ���Ϊt,�����صĴ���gǰ������T��
		    t,	//	ʱ���	ȡӢ��time�ĵ�1����ĸ��
		    u,	//	����		ȡӢ������auxiliary �ĵ�2����ĸ,��a���������ݴʡ�
		    vg,	//	������	���������ء����ʴ���Ϊv�������صĴ���gǰ������V��
		    v,	//	����		ȡӢ�ﶯ��verb�ĵ�һ����ĸ��
		    vd,	//	������	ֱ����״��Ķ��ʡ����ʺ͸��ʵĴ��벢��һ��
		    vn,	//	������	ָ�������ʹ��ܵĶ��ʡ����ʺ����ʵĴ��벢��һ��
		    w,	//	������   
		    x,	//	��������	��������ֻ��һ�����ţ���ĸxͨ�����ڴ���δ֪�������š�
		    y,	//	������	ȡ���֡������ĸ��
		    z,	//	״̬��	ȡ���֡�״������ĸ��ǰһ����ĸ��
			PKU_Tag_Index_Count
		};
	};
	
}