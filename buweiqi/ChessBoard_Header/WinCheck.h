#ifndef WinCheck_H
#define WinCheck_H
#include <conio.h>

namespace WinCheck {
	class ChessInfo {
	private:
		mutable bool WinCheckPosition[4];
		mutable bool WinCheckCross[10][10];
		mutable int WinCheckcross[10][10];
	protected:
		//�ж϶Է��������Ƿ�Χ��������������ʾ�Է����ӵ�λ��
		//�ú�������һ�����ӱ�Χ����������õݹ�ķ������һ�����ӵ����
		//�����ӱ�Χ�����򷵻�true��ֻҪ������Χ��һ����λ�ͷ���false
		bool Besieged(int RivalLine, int RivalColumn, int player, int rival) const
		{
			bool tie[4] = { false, false, false, false };
			auto setStatus = [&](int RivalLine, int RivalColumn) -> void {
				WinCheckCross[RivalLine][RivalColumn] = true;
				WinCheckCross[RivalLine][0] = true;//�±�Ϊ0����һ�����Ϊtrue���ʼ���������ó�ʼ��
			};
			setStatus(RivalLine, RivalColumn);//����λ�÷���״̬
											  //���Է����ӵ��Ϸ�
			auto CheBesie = [&](int RivalLine, int RivalColumn, int player, int rival, bool &tie)
			{
				//���Է����ӵ�ĳ�����жԷ���������δ����Ե
				if (WinCheckcross[RivalLine][RivalColumn] == rival && RivalLine > 0 && RivalColumn > 0
					&& WinCheckCross[RivalLine][RivalColumn] == false && RivalLine < 10 && RivalColumn < 10)
					tie = Besieged(RivalLine, RivalColumn, player, rival);
				//���Է����ӵ�ĳ����û�����ӣ�ֱ�ӷ���false
				else if (WinCheckcross[RivalLine][RivalColumn] == 0 && RivalLine > 0 && RivalColumn > 0 && RivalLine < 10 && RivalColumn < 10)
					return false;
				//���Է����ӵ�ĳ�����м��������ӻ��ѵ���Ե
				else tie = true;
				return true;
			};
			if (!CheBesie(RivalLine - 1, RivalColumn, player, rival, tie[0])) return false;
			//���Է����ӵ��·�
			if (!CheBesie(RivalLine + 1, RivalColumn, player, rival, tie[1])) return false;
			//���Է����ӵ��ҷ�
			if (!CheBesie(RivalLine, RivalColumn + 1, player, rival, tie[2])) return false;
			//���Է����ӵ���
			if (!CheBesie(RivalLine, RivalColumn - 1, player, rival, tie[3])) return false;
			if (tie[0] && tie[1] && tie[2] && tie[3]) return true;
			return false;
		}
		///������OriLine  ����λ�õ���
		///������OriColumn  ����λ�õ���
		///������CLine  ��Ҫ����λ�õ���
		///������CColumn   ��Ҫ����λ�õ���
		///������Player  �ĸ�����µ�����
		///������i  �±꣨WinCheckPosition��
		bool Check(int OriLine, int OriColumn, int CLine, int CColumn, int player, int i, int& Winner) const
		{
			int rival = player == 1 ? 2 : 1;
			auto reduceRecursionTimes = [&]() -> void {
				for (int i = 0; i < 10; ++i)
				{
					if (WinCheckCross[i][0] == true)
						for (int j = 0; j < 10; ++j)
							WinCheckCross[i][j] = false;
				}
			};
			/*
			auto showChessBroadInfoOnDOS = [&]()
			{
				int countB = 0;
				int countW = 0;
				_cprintf("------------------Chess Broad---------------------\n");
				for (int i = 1; i < 10; ++i)
				{
					for (int j = 1; j < 10; ++j)
					{
						_cprintf("%d\t", WinCheckcross[i][j]);
						if (WinCheckcross[i][j] == isWhite)
						{
							countW++;
						}
						else if (WinCheckcross[i][j] == isBlack)
						{
							countB++;
						}
					}
					_cprintf("\n");
				}
				_cprintf("----Black:%d,White:%d----\n", countB, countW);
			};*/
			//�����������ѵ���Ե
			if (CColumn == 0 || CColumn == 10 || CLine == 0 || CLine == 10) WinCheckPosition[i] = true;
			//���жԷ���һ������
			else if (WinCheckcross[CLine][CColumn] == rival)
			{
				reduceRecursionTimes();
				if (Besieged(CLine, CColumn, player, rival))
				{
					Winner = rival;
					//showChessBroadInfoOnDOS();
					return true;
				}
				WinCheckPosition[i] = true;
			}
			//���м���������
			else if (WinCheckcross[CLine][CColumn] == player)
			{
				reduceRecursionTimes();
				WinCheckPosition[i] = Besieged(OriLine, OriColumn, rival, player);
			}
			return false;
		}
	public:
		/*ʤ������
		* 1����ʱ���������ж��ɴ��ڹ��̺���ʵ��
		* 2�����֡��Ⱥ���
		* 3��Χ��������˼�룺���������һ�����ӣ����жϸú�����Χ���ĸ�λ������û�а��ӱ�Χ�������иú����Ƿ�Χ��
		*/
		//ע�����øú��������������ң����Ǹ��˻�AI�ǡ�������
		bool WinOrLose(int& line, int& column, int& onTurn, int& Winner, int c[10][10]) const
		{
			auto initArray = [&](int c[10][10]) {
				for (int i = 0; i < 10; ++i) {
					for (int j = 0; j < 10; ++j) {
						WinCheckcross[i][j] = c[i][j];
						WinCheckCross[i][j] = false;
					}
				}
			};
			initArray(c);

			int i;
			for (i = 0; i < 4; ++i) WinCheckPosition[i] = false;
			int player;//��¼�����ı�ţ���1����2��
			int rival;//��¼�Է��ı�ţ���1����2��
					  //onTurn��ֵ���ȱ仯���ٵ��øú�����WinOrLose����
			if (onTurn == isPlay1onTurn || onTurn == isAI1onTurn)
			{
				player = isWhite;
				rival = isBlack;
			}
			else if (onTurn == isPlay2onTurn || onTurn == isAI2onTurn)
			{
				player = isBlack;
				rival = isWhite;
			}
			else {
				return false;
			}

			//�ж�����������û������
			//������
			if (Check(line, column, line, column - 1, player, 0, Winner))
			{
				Winner = rival;
				return true;
			}
			//����ұ�
			if (Check(line, column, line, column + 1, player, 1, Winner))
			{
				Winner = rival;
				return true;
			}
			//����ϱ�
			if (Check(line, column, line - 1, column, player, 2, Winner))
			{
				Winner = rival;
				return true;
			}
			//����±�
			if (Check(line, column, line + 1, column, player, 3, Winner))
			{
				Winner = rival;
				return true;
			}
			if (WinCheckPosition[0] && WinCheckPosition[1] && WinCheckPosition[2] && WinCheckPosition[3])
			{
				Winner = rival;
				return true;
			}
			return false;
		}
	};

};
#endif // WinCheck_H
