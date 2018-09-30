#pragma once

namespace LogProgress
{
	public ref class LogProgressBar : public System::Windows::Forms::ProgressBar
	{
	public:
		LogProgressBar(void)
		{
			this->DoubleBuffered = true;
			this->SetStyle(	System::Windows::Forms::ControlStyles::AllPaintingInWmPaint |
							System::Windows::Forms::ControlStyles::UserPaint			|
							System::Windows::Forms::ControlStyles::OptimizedDoubleBuffer, true);
		}

	protected:
		void OnPaint(System::Windows::Forms::PaintEventArgs^ e) override
		{
			System::Drawing::Rectangle ClientRectangle	= this->ClientRectangle;
			System::Drawing::Graphics^ Graphics			= e->Graphics;

			System::Windows::Forms::ProgressBarRenderer::DrawHorizontalBar(Graphics, ClientRectangle);
			ClientRectangle.Inflate(0, 0);
			if (this->Value > 0)
			{
				System::Drawing::Rectangle clip = System::Drawing::Rectangle(ClientRectangle.X, ClientRectangle.Y, (int)System::Math::Round(((float)this->Value / this->Maximum) * ClientRectangle.Width), ClientRectangle.Height);
				System::Windows::Forms::ProgressBarRenderer::DrawHorizontalChunks(Graphics, clip);
			}

			System::String^ Draw_String = this->Value.ToString() + " / " + this->Maximum.ToString();
//			System::String^ Draw_String = ((this->Value * 100) / this->Maximum).ToString() + "%";

			System::Drawing::Font^ Draw_String_Font		= System::Drawing::SystemFonts::DefaultFont;
			System::Drawing::SizeF Draw_String_Size		= Graphics->MeasureString(Draw_String, Draw_String_Font);
			System::Drawing::Point Draw_String_Location = System::Drawing::Point((int)((ClientRectangle.Width / 2) - (Draw_String_Size.Width / 2)), (int)((ClientRectangle.Height / 2) - (Draw_String_Size.Height / 2)));

			Graphics->DrawString(Draw_String, Draw_String_Font, System::Drawing::Brushes::Black, Draw_String_Location);
		}
	};
	
	public ref class LogProgress : public System::Windows::Forms::Form
	{
	private:
		LogProgress::LogProgressBar^			_Progress_Bar;

	public:
		LogProgress(void);

		void Set_Title(System::String^ title);
		void Set_Progress_Maximum(unsigned int maximum);
		void Set_Progress_Value(unsigned int value);

	};
}
