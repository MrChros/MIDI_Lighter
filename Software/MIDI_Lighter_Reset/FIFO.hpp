#ifndef FIFO_HPP_
#define FIFO_HPP_

namespace HAL
{
	template <class Data_Type>
	class FIFO
	{
		public:
			FIFO(unsigned int size);
			~FIFO();

			Data_Type Read(bool pull = true);
			Data_Type Preview(unsigned int fifo_position = 0);
			void Write(Data_Type value);

			bool Full() const;
			bool Empty() const;
			unsigned int Fill_Count() const;
			unsigned int Size() const;

			void Clear();
			void Clear_Read();
			void Clear_Write();

		private:
			Data_Type* _Data;

			unsigned int _Size;
			unsigned int _Read_Pointer;
			unsigned int _Write_Pointer;
	};
}


template <class Data_Type>
inline HAL::FIFO<Data_Type>::FIFO(unsigned int size)
{
	_Size = size + 1;
	_Data = new Data_Type[_Size];

	_Read_Pointer = 0;
	_Write_Pointer = 0;
}

template <class Data_Type>
inline HAL::FIFO<Data_Type>::~FIFO()
{
	delete [] _Data;
}

template <class Data_Type>
inline Data_Type HAL::FIFO<Data_Type>::Read(bool pull)
{
	Data_Type Return_Data = _Data[_Read_Pointer];
	
	if (this->Fill_Count() > 0)
	{
		if (pull == true)
		{
			_Read_Pointer++;
			if (_Read_Pointer == _Size)
			{
				_Read_Pointer = 0;
			}
		}
	}

	return Return_Data;
}

template <class Data_Type>
inline Data_Type HAL::FIFO<Data_Type>::Preview(unsigned int fifo_position)
{
	unsigned int Index = fifo_position;
	if (Index >= _Size - 2)
	{
		Index = _Size - 2;
	}
	
	if (_Read_Pointer + Index < _Size)
	{
		return _Data[_Read_Pointer + Index];
	}
	else
	{
		return _Data[(_Read_Pointer + Index) - _Size];
	}
}

template <class Data_Type>
inline void HAL::FIFO<Data_Type>::Write(Data_Type value)
{
	if (!this->Full())
	{
		_Data[_Write_Pointer++] = value;
		if (_Write_Pointer == _Size)
		{
		 	_Write_Pointer = 0;
		}
	}
}

template <class Data_Type>
inline bool HAL::FIFO<Data_Type>::Full() const
{
	if (this->Fill_Count() == _Size - 1)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class Data_Type>
inline bool HAL::FIFO<Data_Type>::Empty() const
{
	if (this->Fill_Count() == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <class Data_Type>
inline unsigned int HAL::FIFO<Data_Type>::Fill_Count() const
{
	if (_Write_Pointer >= _Read_Pointer)
	{
		return _Write_Pointer - _Read_Pointer;
	}
	else
	{
		return (_Size + _Write_Pointer) - _Read_Pointer;
	}
}

template <class Data_Type>
inline unsigned int HAL::FIFO<Data_Type>::Size() const
{
	return _Size - 1;
}

template <class Data_Type>
inline void HAL::FIFO<Data_Type>::Clear()
{
	_Read_Pointer = 0;
	_Write_Pointer = 0;
}

template <class Data_Type>
inline void HAL::FIFO<Data_Type>::Clear_Read()
{
	_Read_Pointer = _Write_Pointer;
}

template <class Data_Type>
inline void HAL::FIFO<Data_Type>::Clear_Write()
{
	_Write_Pointer = _Read_Pointer;
}

#endif