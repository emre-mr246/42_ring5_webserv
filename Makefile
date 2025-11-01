NAME        = webserv

CXX         = c++
CXXFLAGS    = -Wall -Wextra -Werror -std=c++98 -pedantic -g
RM          = rm -rf

SRCS        = main.cpp serverSetup.cpp error.cpp serverExtract.cpp printConfig.cpp printConfig2.cpp location.cpp location2.cpp parserUtils.cpp parserAddressError.cpp parser2.cpp parser.cpp parser3.cpp utils.cpp pollManager.cpp connectionHandler.cpp connectionHandler2.cpp connectionHandler3.cpp connectionHandler4.cpp eventLoop.cpp socket.cpp socketBind.cpp config.cpp validation.cpp requestParser.cpp headerParser.cpp headerParser2.cpp responseBuilder.cpp requestPrinter.cpp responsePrinter.cpp requestProcessor.cpp chunkedDecoder.cpp requestValidator.cpp errorResponse.cpp errorResponse2.cpp responseSender.cpp responseQueue.cpp responseQueues.cpp pathResolver.cpp hostParser.cpp hostParser2.cpp serverMatcher.cpp locationResolver.cpp fileReader.cpp mimeType.cpp getHandler.cpp postHandler.cpp deleteHandler.cpp putHandler.cpp fileWriter.cpp clientTimeout.cpp cgiTimeout.cpp cgiExecutor.cpp cgiExecutor2.cpp cgiExecutor3.cpp cgiExecutor4.cpp cgiHelper.cpp clientBufferManager.cpp clientStateManager.cpp connectionValidator.cpp connectionValidationHelper.cpp
HDRS        = webserv.hpp http.hpp

vpath %.cpp src src/config src/http src/cgi
vpath %.hpp inc

all: $(NAME)

$(NAME): $(SRCS) $(HDRS)
	@$(CXX) $(CXXFLAGS) -I inc/ $(filter %.cpp, $^) -o $@
	@echo "$(DARKBLUE)-== $(NAME) created! ツ ==-$(DEFAULT)"

clean:
	@pkill webserv 2>/dev/null || true
	@echo "$(YELLOW)-== connections closed! ==-$(DEFAULT)"

fclean: clean
	@$(RM) $(NAME) 2>/dev/null || true
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
