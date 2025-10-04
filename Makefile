NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -pedantic -g
RM          = rm -rf

SRCS        = main.cpp error.cpp serverExtract.cpp printConfig.cpp location.cpp location2.cpp parserUtils.cpp parserAddressError.cpp parser2.cpp debug.cpp parser.cpp utils.cpp pollManager.cpp connectionHandler.cpp eventLoop.cpp socket.cpp config.cpp validation.cpp requestParser.cpp headerParser.cpp responseBuilder.cpp staticFileHandler.cpp requestPrinter.cpp requestProcessor.cpp
HDRS        = webserv.hpp http.hpp

vpath %.cpp src src/config src/debug src/http
vpath %.hpp inc

all: $(NAME)

$(NAME): $(SRCS) $(HDRS)
	@$(CXX) $(CXXFLAGS) -I inc/ $(filter %.cpp, $^) -o $@
	@echo "$(DARKBLUE)-== $(NAME) created! ツ ==-$(DEFAULT)"

clean:
	@echo "$(YELLOW)-== no object files to clean ==-$(DEFAULT)"

fclean:
	@$(RM) $(NAME)
	@echo "$(RED)-== all files deleted! ==-$(DEFAULT)"

re: fclean all

.PHONY: all clean fclean re

#ANSI COLORS
DEFAULT = \033[0m
RED     = \033[1;31m
YELLOW  = \033[1;33m
GREEN   = \033[1;32m
BLUE    = \033[1;36m
DARKBLUE = \033[1;34m
ORANGE  = \033[38;5;208m