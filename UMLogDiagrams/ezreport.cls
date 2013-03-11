%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Process arguments and load external packages
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Document class declaration
\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{ezreport}[2011/11/08 v0.05 Document class for easily writing technical reports.]

% Load packages needed within the document class
\RequirePackage{etoolbox}
\RequirePackage{kvoptions}
\SetupKeyvalOptions{family=ML,prefix=ML@}

% Provide default options
\PassOptionsToClass{a4paper,twoside,11pt}{memoir}

% Process arguments
\DeclareStringOption[english]{language}
\ProcessKeyvalOptions*
\PassOptionsToPackage{\ML@language}{babel}

\DeclareStringOption[utf8x]{encoding}
\ProcessKeyvalOptions*
\PassOptionsToPackage{\ML@encoding}{inputenc}

\DeclareOption{plain}{\def\@plain{true}}

\DeclareOption{dense}
{
	\PassOptionsToClass{openany}{memoir}
}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{memoir}}
\ProcessOptions\relax

% Load the base class and needed packages
\LoadClass{memoir}
\RequirePackage{babel}
\RequirePackage{ucs}
\RequirePackage{inputenc}
\RequirePackage[T1]{fontenc}
\RequirePackage[bindingoffset=1cm]{geometry}
\RequirePackage[protrusion=true,expansion=true]{microtype}
\RequirePackage[pdftex,ocgcolorlinks,colorlinks,linkcolor=red,citecolor=blue,urlcolor=blue,linktoc=page]{hyperref}
\RequirePackage{makeidx}
\RequirePackage[all]{hypcap}
\RequirePackage[pdftex]{graphicx}
\RequirePackage{float}
\RequirePackage{fixltx2e}
\RequirePackage{amsmath}
\RequirePackage{amsfonts}
\RequirePackage{amssymb}
\RequirePackage{listings}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Customize the formatting of the document
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Titlepage formatting
\pretitle
{
	\begin{center}
	\huge
	\bfseries
	\scshape
}

\posttitle
{
	\par
	\vskip 0.5em
	\end{center}
}

\preauthor
{
	\begin{center}
	\LARGE
	\lineskip 0.5em
	\begin{tabular}[t]{c}
}

\postauthor
{
	\end{tabular}
	\par
	\end{center}
}

\predate
{
	\begin{center}
	\Large
}

\postdate
{
	\par
	\end{center}
}


% Page formatting
\makepagestyle{ruled}
\makeevenfoot{ruled}{}{}{}
\makeoddfoot{ruled}{}{}{}
\makeheadrule{ruled}{\textwidth}{\normalrulethickness}
\makeevenhead{ruled}{\scshape\leftmark}{}{\thepage}
\makeoddhead{ruled}{\thepage}{}{\scshape\rightmark}
\makepsmarks{ruled}{
	\nouppercaseheads
	\createmark{chapter}{right}{nonumber}{}{}
	\createmark{section}{left}{nonumber}{}{}
	\createplainmark{toc}{both}{\contentsname}
	\createplainmark{lof}{both}{\listfigurename}
	\createplainmark{lot}{both}{\listtablename}
	\createplainmark{bib}{both}{\bibname}
	\createplainmark{index}{both}{\indexname}
	\createplainmark{glossary}{both}{\glossaryname}
}

\pagestyle{ruled}

\aliaspagestyle{chapter}{ruled}
\aliaspagestyle{title}{empty}

% Chapter formatting
\chapterstyle{hangnum}
\renewcommand{\chapnumfont}{\LARGE\bfseries\scshape}
\renewcommand{\chaptitlefont}{\LARGE\bfseries\scshape}
\renewcommand{\beforechapskip}{0.5em}
\renewcommand{\afterchapskip}{0.7em}

% Section formatting
\setsecheadstyle{\Large\raggedright\scshape}
\setsubsecheadstyle{\large\raggedright\scshape}
\setsubsubsecheadstyle{\raggedright\scshape}

% Caption formatting
\captionstyle{\centering}
\captionnamefont{\small\scshape}
\captiontitlefont{\small}

% Numerical structure
%\numberwithin{figure}{chapter}
%\numberwithin{table}{chapter}
%\numberwithin{equation}{chapter}

% Metadata
\hypersetup
{
	pdftitle=\@title,
	pdfauthor=\@author,
	pdfduplex=DuplexFlipLongEdge,
	pdfpagemode=UseOutlines
}

% Internalization of internal names
\newcommand{\codename}{Code}
\addto\captionsnorsk
{
	\renewcommand{\codename}{Kode}
}



%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Define custom macros and environments
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Macro for including figures
\newcommand{\includefigure}[4][\@empty]{
	\begin{figure}[H]
	\centering

	\ifx \@empty#1
		\includegraphics[width=0.7\textwidth]{#2}
	\else
		\includegraphics[#1]{#2}
	\fi

	\caption{#4}
	\label{#3}
	\end{figure}
}

% Improved table environment
\let\oldtable=\table
\let\endoldtable=\endtable
\newcommand{\tablesection}{\midrule}
\renewenvironment{table}[3]
{
	\begin{oldtable}[h!]
	\centering
	\caption{#3}
	\label{#2}
	\begin{tabular}{#1}
	\toprule
}
{
	\bottomrule
	\end{tabular}
	\end{oldtable}
}

% Code environment
\renewcommand\lstlistingname{\codename}
\renewcommand\lstlistlistingname{\codename}
\lstnewenvironment{code}[3]{\lstset{numbers=right,language=#1,label=#2,caption=#3}}{}

% References
\newcommand{\figref}[1]{\small\textsc{\figurename~\ref{#1}}}
\newcommand{\tabref}[1]{\small\textsc{\tablename~\ref{#1}}}
\newcommand{\coderef}[1]{\small\textsc{\codename~\ref{#1}}}

% Abstract environment
\let\oldabstract=\abstract
\let\endoldabstract=\endabstract
\newcommand{\theabstract}{}

\renewcommand{\abstract}[1]
{
	\renewcommand{\theabstract}
	{
		\begin{oldabstract}
			\noindent\ignorespaces#1
		\end{oldabstract}
		\clearpage
	}
}


%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
%
%   Add default elements to the document structure
%
%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

% Enable index generation
% \makeindex

% Document structure depends on
\ifx\@plain\undefined
{
	% Beginning of document
	\AtBeginDocument{
		% Settings
		\numberwithin{lstlisting}{chapter}
	
		% Frontmatter
		\frontmatter
		\maketitle
		%\cleardoublepage
		%\theabstract
		%\cleardoublepage
		%\tableofcontents*
		%\cleardoublepage
	
		% Mainmatter
		\mainmatter
		\setcounter{secnumdepth}{2}
	}

	% End of document
	\AtEndDocument
	{
		\backmatter
		\printindex
	}
}
\fi
