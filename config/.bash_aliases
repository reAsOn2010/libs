alias alert='notify-send --urgency=low -i "$([ $? = 0 ] && echo terminal || echo error)" "$(history|tail -n1|sed -e '\''s/^\s*[0-9]\+\s*//;s/[;&|]\s*alert$//'\'')"'

 alias rm='rm -i'
 alias cp='cp -i'
 alias mv='mv -i'
#
# Default to human readable figures
 alias df='df -h'
 alias du='du -h'
#
# Misc :)
 alias less='less -r'                          # raw control characters
 alias whence='type -a'                        # where, of a sort
 alias grep='grep --color'                     # show differences in colour
 alias egrep='egrep --color=auto'              # show differences in colour
 alias fgrep='fgrep --color=auto'              # show differences in colour

# Some shortcuts for different directory listings
 alias ls='ls -hF --color=tty'                 # classify files in colour
 alias dir='ls --color=auto --format=vertical'
 alias vdir='ls --color=auto --format=long'
 alias ll='ls -l'                              # long list
 alias la='ls -A'                              # all but . and ..
 alias l='ls -CF'                              #
 alias lr='ls -R'
