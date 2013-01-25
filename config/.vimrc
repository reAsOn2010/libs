" An example for a vimrc file.
"
" Maintainer:	Bram Moolenaar <Bram@vim.org>
" Last change:	2008 Dec 17
"
" To use it, copy it to
"     for Unix and OS/2:  ~/.vimrc
"	      for Amiga:  s:.vimrc
"  for MS-DOS and Win32:  $VIM\_vimrc
"	    for OpenVMS:  sys$login:.vimrc

" When started as "evim", evim.vim will already have done these settings.
if v:progname =~? "evim"
  finish
endif

" Use Vim settings, rather than Vi settings (much better!).
" This must be first, because it changes other options as a side effect.
set nocompatible

" allow backspacing over everything in insert mode
set backspace=indent,eol,start

if has("vms")
  set nobackup		" do not keep a backup file, use versions instead
else
  set backup		" keep a backup file
endif
set history=50		" keep 50 lines of command line history
set ruler		" show the cursor position all the time
set showcmd		" display incomplete commands
set incsearch		" do incremental searching

" For Win32 GUI: remove 't' flag from 'guioptions': no tearoff menu entries
" let &guioptions = substitute(&guioptions, "t", "", "g")

" Don't use Ex mode, use Q for formatting
map Q gq

" CTRL-U in insert mode deletes a lot.  Use CTRL-G u to first break undo,
" so that you can undo CTRL-U after inserting a line break.
inoremap <C-U> <C-G>u<C-U>

" In many terminal emulators the mouse works just fine, thus enable it.
if has('mouse')
  set mouse=a
endif

" Switch syntax highlighting on, when the terminal has colors
" Also switch on highlighting the last used search pattern.
if &t_Co > 2 || has("gui_running")
  syntax on
  set hlsearch
endif

" Only do this part when compiled with support for autocommands.
if has("autocmd")

  " Enable file type detection.
  " Use the default filetype settings, so that mail gets 'tw' set to 72,
  " 'cindent' is on in C files, etc.
  " Also load indent files, to automatically do language-dependent indenting.
  filetype plugin indent on

  " Put these in an autocmd group, so that we can delete them easily.
  augroup vimrcEx
  au!

  " For all text files set 'textwidth' to 78 characters.
  autocmd FileType text setlocal textwidth=78

  " When editing a file, always jump to the last known cursor position.
  " Don't do it when the position is invalid or when inside an event handler
  " (happens when dropping a file on gvim).
  " Also don't do it when the mark is in the first line, that is the default
  " position when opening a file.
  autocmd BufReadPost *
    \ if line("'\"") > 1 && line("'\"") <= line("$") |
    \   exe "normal! g`\"" |
    \ endif

  augroup END

else

  set autoindent		" always set autoindenting on

endif " has("autocmd")

" Convenient command to see the difference between the current buffer and the
" file it was loaded from, thus the changes you made.
" Only define it when not defined already.
if !exists(":DiffOrig")
    command DiffOrig vert new | set bt=nofile | r # | 0d_ | diffthis
		  \ | wincmd p | diffthis
endif

" ------------------------------------
" Leon Zhou's .vimrc begins
" ------------------------------------
" color theme for terminal
color ron

" ru
set ru

" is
set is

" display code
set fenc=utf-8
set fencs=utf-8,usc-bom,euc-jp,gb18030,gbk,gb2312,cp936

" Comfirm when Unsaved or Read-only
set confirm

" no backup
set nobackup

" 高亮字符，让其不受100列限制
highlight OverLength ctermbg=red ctermfg=white guibg=red guifg=white
match OverLength '\%101v.*'

" 光标移动到buffer的顶部和底部时保持3行距离
set scrolloff=3

" 在被分割的窗口间显示空白，便于阅读
set fillchars=vert:\ ,stl:\ ,stlnc:\

" 命令行（在状态行下）的高度，默认为1
set cmdheight=1


" 我的状态行显示的内容（包括文件类型和解码）
set statusline=%F%m%r%h%w\ [FORMAT=%{&ff}]\ [TYPE=%Y]\ [POS=%l,%v][%p%%]\ %{strftime(\"%d/%m/%y\ -\ %H:%M\")}

" 总是显示状态行
set laststatus=2

" 制表符为4
set tabstop=4

" 统一缩进为4
set softtabstop=4
set shiftwidth=4

" display tab and space as some other remarkable characters
set list
set listchars=tab:>.,trail:.

" tab menu
set wildmenu

" get support of color
set t_Co=256

" highlight cursor
set cursorcolumn
hi CursorLine cterm=NONE ctermbg=236 ctermfg=NONE guibg=purple guibg=NONE
set cursorline
hi CursorColumn cterm=NONE ctermbg=236 ctermfg=NONE guibg=purple guibg=NONE

" set tag
set tags=/home/reason/Documents/git/tags

" for plugin taglist
let Tlist_Show_One_File=1
let Tlist_Exit_OnlyWindow=1


" combine Taglist and NERDtree
let g:NERDTree_title="[NERD Tree]" 
let g:winManagerWindowLayout='NERDTree|TagList,BufExplorer'
function! NERDTree_Start()
    exec 'NERDTree'
endfunction
function! NERDTree_IsValid()
    return 1
endfunction
"nmap wm :if IsWinManagerVisible() <BAR> WMToggle<CR> <BAR> else <BAR> WMToggle<CR>:q<CR> endif <CR><CR>
nmap <F3> :if IsWinManagerVisible() <BAR> WMToggle<CR> <BAR> else <BAR> WMToggle<CR>:q<CR> endif <CR><CR>




" combine Taglist and netrw
" let g:winManagerWindowLayout='FileExplorer|TagList'
" nmap wm :WMToggle<cr>

" for MiniBufExplorer
let g:miniBufExplMapWindowNavVim = 1
let g:miniBufExplMapWindowNavArrows = 1

""""""""""""""""""""""""""""""
" netrw setting
""""""""""""""""""""""""""""""
let g:netrw_winsize = 20
nmap fe :Sexplore!<cr>

" for new-omni-completion
set completeopt=longest,menu

" for closetag
let g:closetag_html_style=1
source ~/.vim/scripts/closetag.vim

" check spelling
" :setlocal spell spelllang=en_us

if has("autocmd")
   autocmd FileType xml,html,c,cs,java,perl,shell,bash,cpp,python,vim,php,ruby,tex,javascript,make,octave,go set number
   autocmd FileType xml,html,c,cs,java,perl,shell,bash,cpp,python,vim,php,ruby,tex,javascript,htmldjango,lua,octave,go set expandtab
   autocmd FileType xml,html,c,cs,java,perl,shell,bash,cpp,python,vim,php,ruby,tex,javascript,htmldjango,lua,octave,go set nowrap
   autocmd FileType xml,html vmap <C-o> <ESC>'<i<!--<ESC>o<ESC>'>o-->
   autocmd FileType java,c,cpp,cs vmap <C-o> <ESC>'<o/*<ESC>'>o*/
   autocmd FileType html,text,php,vim,c,java,xml,bash,shell,perl,python setlocal textwidth=100
   autocmd BufReadPost *
      \ if line("'\"") > 0 && line("'\"") <= line("$") |
      \   exe "normal g`\"" |
      \ endif
endif " has("autocmd")
