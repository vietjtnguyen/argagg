Name:           argagg
Version:        0.1.1
Release:        1%{?dist}
Summary:        Simple C++ command line argument/option parser

License:        Proprietary
URL:            https://github.com/vietjtnguyen/argagg/
Source0:        https://github.com/vietjtnguyen/argagg/archive/%{version}.tar.gz#/%{name}-%{version}.tar.gz

BuildRequires:  cmake
BuildRequires:  doxygen

%description
This is yet another C++ command line argument/option parser. It was written as
a simple and idiomatic alternative to other frameworks like getopt, Boost
program options, TCLAP, and others. The goal is to achieve the majority of
argument parsing needs in a very simple manner (simple mental model) and easy
to use API. It operates as a single pass over all arguments, recognizing flags
prepended by `-` (short) or `--` (long) and aggregating them into easy to
access structures.

%package        devel
Summary:        Development files for %{name}
Requires:       %{name}%{?_isa} = %{version}-%{release}

%description    devel
The %{name}-devel package contains the header files for developing applications
that use %{name}.

%prep
%setup -q

%build
%cmake
make %{?_smp_mflags}

%install
%make_install

%check
ctest -V %{?_smp_mflags}

%files devel
%doc %{_datadir}/doc/%{name}
%{_includedir}/*

%changelog
* Sun Jan 29 2017 Viet The Nguyen <vietjtnguyen@gmail.com>
- Initial packaging
