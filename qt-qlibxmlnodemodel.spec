Name:           qt-qlibxmlnodemodel
Version:        0.1
Release:        1%{?dist}
Summary:        A QAbstractXmlNodeModel for using with libxml2 library

License:        BSD
URL:            https://github.com/atorkhov/qlibxmlnodemodel
Source0:        qlibxmlnodemodel-0.1.tar.bz2

BuildRequires:  cmake qt-devel libxml2-devel
# For qt4 and libxml2 cmake modules:
BuildRequires:  kdelibs-devel

%description
qlibxmlnodemodel is a QAbstractXmlNodeModel implementation for libxml2 library.
It uses libxml2 for HTML parsing and offers the document as a custom XML node
model that can be used with Qt's XmlPatterns module. This allows you to use
XPath, XQuery, XSLT and other fun stuff by standard QT means on HTML documents
that aren't valid XML.


%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains libraries and header files for
developing applications that use %{name}.


%prep
%setup -q -n qlibxmlnodemodel


%build
%cmake .
make %{?_smp_mflags}


%install
rm -rf $RPM_BUILD_ROOT
make install DESTDIR=$RPM_BUILD_ROOT
find $RPM_BUILD_ROOT -name '*.la' -exec rm -f {} ';'


%post -p /sbin/ldconfig

%postun -p /sbin/ldconfig


%files
%doc COPYING README
%{_libdir}/*.so.*

%files devel
%doc COPYING README
%{_includedir}/*
%{_libdir}/*.so


%changelog
* Fri Jan 13 2012 Alexey Torkhov <atorkhov@gmail.com> - 0.1-1
- Initial package.

