<?xml version="1.0" encoding="windows-1251"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
<xsl:output method="html" omit-xml-declaration="yes" indent="yes"/>
<xsl:template match="/">
<html>
<head>
<title><xsl:value-of select="gauge/@name"/></title>
<meta>
<xsl:attribute name="name">author</xsl:attribute>
<xsl:attribute name="value"><xsl:value-of select="@author"/></xsl:attribute>
</meta>
<style>
li {font-family:Helvetica,Verdana; font-size:14px};
p {font-family:Times New Roman;font-size:16px};
div {font-family:Times New Roman;font-size:16px};
td {font-family:Helvetica,Verdana; font-size:14px};

</style>
</head>
<body>
<xsl:apply-templates select="gauge"/>
</body>
</html>
</xsl:template>
<xsl:template match="gauge">
<h1><xsl:value-of select="description/@fullname"/></h1>
<div><xsl:value-of disable-output-escaping="yes" select="description/data"/></div>
<div>
<h3>Основные эксплуатационно-технические характеристики</h3>
<table bgcolor="#909090" border="0" cellpadding="0" cellspacing="0">
<tr><td>
<table border="0" cellpadding="2" cellspacing="1">
<xsl:for-each select="ttc/item">
<tr>
<td valign="top" bgcolor="#FFFFFF"><xsl:value-of select="position()"/></td>
<xsl:choose>
<xsl:when test="subitem">
<td bgcolor="#FFFFFF" colspan="2">
<table bgcolor="#909090" border="0" cellpadding="0" cellspacing="0">
<tr><td>
<table border="0" cellpadding="2" cellspacing="1">
<tr>
<td bgcolor="#FFFFFF" width="600"><xsl:value-of select="@name"/></td>
<td bgcolor="#FFFFFF">&#160;</td>
</tr>
<xsl:for-each select="subitem">
<tr>
<td width="600" bgcolor="#FFFFFF"><xsl:value-of select="@name"/></td>
<td width="600" bgcolor="#FFFFFF"><xsl:value-of disable-output-escaping="yes" select="@value"/></td>
</tr>
</xsl:for-each>
</table>
</td></tr>
</table>
</td>
</xsl:when>
<xsl:otherwise>
<td width="600" bgcolor="#FFFFFF"><xsl:value-of select="@name"/></td>
<td width="600" bgcolor="#FFFFFF"><xsl:value-of disable-output-escaping="yes" select="@value"/></td>
</xsl:otherwise>
</xsl:choose>
</tr>
</xsl:for-each>
</table>
</td></tr>
</table>
</div>
<h3>Прицип работы</h3>
<div>
<xsl:apply-templates select="work-principe"/>
</div>
<h3>Включение, проверка работоспособности и эксплуатация РЛС в полете</h3>
<div>
<xsl:apply-templates select="profile"/>
</div>
<h3>Установка и подключение</h3>
<div>
<xsl:apply-templates select="installation"/>
</div>
<h3>Особенности реализации в симуляторе</h3>
<div>
<ul>
<xsl:for-each select="sim-features/item">
<li><xsl:value-of disable-output-escaping="yes" select="."/></li>
</xsl:for-each>
</ul>
</div>
<div>
С уважением, <xsl:value-of select="@author-ru"/>
</div>
</xsl:template>
<xsl:template match="work-principe">
<p><xsl:value-of select="generic"/></p>
<xsl:for-each select="mode">
<p><xsl:value-of select="position()"/>.<b>Режим &quot;<xsl:value-of select="@name"/>&quot;</b><xsl:value-of select="."/></p>
</xsl:for-each>
</xsl:template>
<xsl:template match="profile">
<p><i><u>П р е д у п р е ж д е н и е.</u><xsl:value-of select="high-radiation-warning"/></i></p>
<p><xsl:value-of select="pre"/></p>
<xsl:for-each select="step">
<p>
<b><xsl:value-of select="@name"/></b>.<br />
<xsl:value-of disable-output-escaping="yes" select="."/>
</p>
</xsl:for-each>
</xsl:template>
<xsl:template match="installation">
Для установки РЛС необходимы следующие файлы:
<ul>
<xsl:for-each select="files/file">
<li><xsl:value-of select="@name"/></li>
</xsl:for-each>
</ul>
Пакет содержит следующие приборы:
<ul>
<xsl:for-each select="gauges/gauge">
<li><xsl:value-of select="@file"/>!<xsl:value-of select="@name"/> - <xsl:value-of select="."/></li>
</xsl:for-each>
</ul>
См. также: 
<a>
<xsl:attribute name="href"><xsl:value-of select="sample-panel-cfg/@file"/></xsl:attribute>
<xsl:value-of select="sample-panel-cfg"/>
</a>
</xsl:template>
</xsl:stylesheet>